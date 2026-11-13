package org.raymanrecon.port;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.DocumentsContract;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/*
 * First start (port/docs/ANDROID.md): the player picks the folder of their game once - GOG's "Rayman Forever"
 * folder, or a Rayman v1.21 game folder (the one with RAY.LNG) - with Android's folder picker. The files the game
 * needs are copied into the app's own folder (Android/data/<package>/files/), laid out like GOG's install so the
 * native side finds them as on Windows (port/game/entry.c): the game in Rayman/, GOG's CD (game.ins, game.gog,
 * Music/) and cloud_saves/ next to it. From then on the game starts directly (RaymanActivity).
 */
public class SetupActivity extends Activity {
    private static final int PICK_FOLDER = 1;
    private static final String GAME = "Rayman";

    private final Handler ui = new Handler(Looper.getMainLooper());
    private AlertDialog progressDialog;
    private ProgressBar progressBar;
    private TextView progressText;

    /* One item of the picked folder. */
    private static final class Item {
        final String id, name;
        final boolean dir;
        final long size;
        Item(String id, String name, boolean dir, long size) {
            this.id = id; this.name = name; this.dir = dir; this.size = size;
        }
    }

    /* One file to copy: a document and its place under the app's folder. */
    private static final class Copy {
        final String id, dest;
        final long size;
        Copy(String id, String dest, long size) { this.id = id; this.dest = dest; this.size = size; }
    }

    static boolean hasGame(File files) {
        return files != null && (new File(files, GAME + "/RAY.LNG").isFile() || new File(files, "RAY.LNG").isFile());
    }

    @Override
    protected void onCreate(Bundle state) {
        super.onCreate(state);
        if (hasGame(getExternalFilesDir(null))) {
            startGame();
            return;
        }
        askForFolder(null);
    }

    private void startGame() {
        startActivity(new Intent(this, RaymanActivity.class));
        finish();
    }

    private void askForFolder(String problem) {
        String text = "Choose the folder of your Rayman (PC version 1.21): GOG's \"Rayman Forever\" folder, or the "
                + "game folder with RAY.LNG.\n\nThe game files, the CD with its music and the saves are copied into "
                + "the app (about 170 MB for GOG).";
        new AlertDialog.Builder(this)
                .setTitle(problem == null ? "Rayman: your game folder" : "Rayman: " + problem)
                .setMessage(text)
                .setCancelable(false)
                .setPositiveButton("Choose folder", (d, w) -> {
                    Intent pick = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
                    startActivityForResult(pick, PICK_FOLDER);
                })
                .setNegativeButton("Quit", (d, w) -> finish())
                .show();
    }

    @Override
    protected void onActivityResult(int request, int result, Intent data) {
        super.onActivityResult(request, result, data);
        if (request != PICK_FOLDER)
            return;
        if (result != RESULT_OK || data == null || data.getData() == null) {
            askForFolder(null);
            return;
        }
        final Uri tree = data.getData();
        showProgress();
        new Thread(() -> {
            String problem;
            try {
                problem = install(tree);
            } catch (Exception e) {
                problem = "copying failed (" + e.getMessage() + ")";
            }
            final String outcome = problem;
            ui.post(() -> {
                if (progressDialog != null)
                    progressDialog.dismiss();
                if (outcome == null)
                    startGame();
                else
                    askForFolder(outcome);
            });
        }, "rayman-setup").start();
    }

    private void showProgress() {
        LinearLayout box = new LinearLayout(this);
        box.setOrientation(LinearLayout.VERTICAL);
        int pad = (int) (24 * getResources().getDisplayMetrics().density);
        box.setPadding(pad, pad, pad, pad);
        progressText = new TextView(this);
        progressText.setText("Looking at the folder...");
        progressBar = new ProgressBar(this, null, android.R.attr.progressBarStyleHorizontal);
        progressBar.setMax(1000);
        box.addView(progressText);
        box.addView(progressBar);
        progressDialog = new AlertDialog.Builder(this).setTitle("Rayman: copying the game").setView(box)
                .setCancelable(false).show();
    }

    private void progress(long done, long total, String name) {
        final int permille = total > 0 ? (int) (done * 1000 / total) : 0;
        final String text = String.format(Locale.ROOT, "%s\n%d of %d MB", name, done >> 20, total >> 20);
        ui.post(() -> {
            if (progressBar != null) progressBar.setProgress(permille);
            if (progressText != null) progressText.setText(text);
        });
    }

    private List<Item> list(Uri tree, String id) {
        List<Item> items = new ArrayList<>();
        Uri children = DocumentsContract.buildChildDocumentsUriUsingTree(tree, id);
        String[] columns = {DocumentsContract.Document.COLUMN_DOCUMENT_ID,
                DocumentsContract.Document.COLUMN_DISPLAY_NAME, DocumentsContract.Document.COLUMN_MIME_TYPE,
                DocumentsContract.Document.COLUMN_SIZE};
        try (Cursor c = getContentResolver().query(children, columns, null, null, null)) {
            while (c != null && c.moveToNext())
                items.add(new Item(c.getString(0), c.getString(1),
                        DocumentsContract.Document.MIME_TYPE_DIR.equals(c.getString(2)), c.isNull(3) ? 0 : c.getLong(3)));
        }
        return items;
    }

    private static Item named(List<Item> items, String name, boolean dir) {
        for (Item i : items)
            if (i.dir == dir && i.name.equalsIgnoreCase(name))
                return i;
        return null;
    }

    /* Programs and shortcuts (RAYMAN.EXE, the Windows port's rayman_sdl3.exe and SDL3.dll, GOG's .lnk files): the
       app reads only the game's data files. */
    private static boolean wanted(String name) {
        String n = name.toLowerCase(Locale.ROOT);
        return !(n.endsWith(".exe") || n.endsWith(".dll") || n.endsWith(".lnk"));
    }

    private void addTree(Uri tree, String id, String dest, List<Copy> out) {
        for (Item i : list(tree, id)) {
            if (i.dir)
                addTree(tree, i.id, dest + "/" + i.name, out);
            else if (wanted(i.name))
                out.add(new Copy(i.id, dest + "/" + i.name, i.size));
        }
    }

    /* null when installed, otherwise what is wrong with the folder. */
    private String install(Uri tree) throws IOException {
        File files = getExternalFilesDir(null);
        if (files == null)
            return "the app's storage is not available";
        delete(new File(files, ".incoming"));   /* left over from an interrupted setup */
        String root = DocumentsContract.getTreeDocumentId(tree);
        List<Item> top = list(tree, root);
        List<Copy> plan = new ArrayList<>();
        if (named(top, "RAY.LNG", false) != null) {
            /* the game folder itself: all of it (its own CD image or movies included) */
            addTree(tree, root, GAME, plan);
        } else {
            Item game = named(top, GAME, true);
            if (game == null || named(list(tree, game.id), "RAY.LNG", false) == null)
                return "no Rayman game found there";
            /* GOG's Rayman Forever: the game, its CD next to it, the saves */
            addTree(tree, game.id, GAME, plan);
            for (Item i : top) {
                String n = i.name.toLowerCase(Locale.ROOT);
                if (!i.dir && (n.endsWith(".ins") || n.endsWith(".cue") || n.endsWith(".gog") || n.endsWith(".bin")))
                    plan.add(new Copy(i.id, i.name, i.size));
                else if (i.dir && (n.equals("music") || n.equals("cloud_saves")))
                    addTree(tree, i.id, i.name, plan);
            }
            new File(files, ".incoming/cloud_saves").mkdirs();
        }
        long total = 0, done = 0;
        for (Copy c : plan)
            total += c.size;
        /* into .incoming first, moved into place when everything is there: no half-copied game is ever started */
        File incoming = new File(files, ".incoming");
        ContentResolver resolver = getContentResolver();
        byte[] buffer = new byte[1 << 20];
        for (Copy c : plan) {
            File dest = new File(incoming, c.dest);
            File parent = dest.getParentFile();
            if (parent != null && !parent.isDirectory() && !parent.mkdirs())
                throw new IOException("cannot create " + parent);
            try (InputStream in = resolver.openInputStream(DocumentsContract.buildDocumentUriUsingTree(tree, c.id));
                 OutputStream out = new FileOutputStream(dest)) {
                if (in == null)
                    throw new IOException("cannot read " + c.dest);
                int n;
                while ((n = in.read(buffer)) > 0) {
                    out.write(buffer, 0, n);
                    done += n;
                    progress(done, total, c.dest);
                }
            }
        }
        File[] parts = incoming.listFiles();
        if (parts != null)
            for (File part : parts) {
                File target = new File(files, part.getName());
                delete(target);
                if (!part.renameTo(target))
                    throw new IOException("cannot move " + part.getName());
            }
        delete(incoming);
        return hasGame(files) ? null : "no Rayman game found there";
    }

    private static void delete(File f) {
        File[] children = f.listFiles();
        if (children != null)
            for (File c : children)
                delete(c);
        f.delete();
    }
}
