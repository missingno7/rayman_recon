package org.raymanrecon.port;

import org.libsdl.app.SDLActivity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

/* SDL's activity runs the port's SDL_main (port/game/entry.c) from libmain.so on its own thread. */
public class RaymanActivity extends SDLActivity {
    @Override
    protected String[] getLibraries() {
        return new String[] { "SDL3", "main" };
    }

    /* Testing (port/docs/ANDROID.md): an optional args.txt in the app's files folder (Android/data/<package>/files/) holds command-line options of the
       desktop build, whitespace-separated (e.g. "--demo 3"). Absent or unreadable: no options. */
    @Override
    protected String[] getArguments() {
        File dir = getExternalFilesDir(null);
        List<String> args = new ArrayList<>();
        if (dir != null) {
            File f = new File(dir, "args.txt");
            if (f.isFile()) {
                try (BufferedReader r = new BufferedReader(
                        new InputStreamReader(new FileInputStream(f), StandardCharsets.UTF_8))) {
                    String line;
                    while ((line = r.readLine()) != null)
                        for (String a : line.trim().split("\\s+"))
                            if (!a.isEmpty())
                                args.add(a);
                } catch (Exception ignored) {
                }
            }
        }
        return args.toArray(new String[0]);
    }
}
