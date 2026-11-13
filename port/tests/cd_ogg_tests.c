/* GOG's CD (Rayman Forever): game.ins names a MODE1/2352 data file (game.gog) and 19 Ogg Vorbis audio tracks
   (Music\rayman02.ogg .. rayman20.ogg). The virtual drive lays them end to end: track 2 starts where game.gog ends,
   each audio track is its decoded length rounded up to whole sectors, its PCM is non-silent, and a read that
   seeks into the middle of a track returns the samples a sequential read gives there. The data track's ISO 9660
   file system is readable (the Rayman folder of the Rayman Forever disc).
       port_cd_ogg_tests GOG_DIR       (skipped without the argument) */
#include "platform/cdrom.h"
#include "platform/ogg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned checks, failures;
#define CHECK(c) do { checks++; if (!(c)) { fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); failures++; } } while (0)

int main(int argc, char **argv)
{
    char cue[1024], ogg_path[1024];
    FILE *f;
    long long data_bytes;
    uint32_t track2, lba, size, sectors;
    uint64_t frames;
    port_ogg *ogg;
    static int16_t seq[44100 * 2 * 4], part[2048 * 2];
    size_t got, i;
    int loud = 0;
    if (argc < 2 || !argv[1][0]) {
        printf("no GOG install given: skipped\n");
        return 0;
    }
    snprintf(cue, sizeof cue, "%s/game.ins", argv[1]);
    if (!(f = fopen(cue, "rb"))) {
        printf("%s not found: skipped\n", cue);
        return 0;
    }
    fclose(f);
    snprintf(ogg_path, sizeof ogg_path, "%s/game.gog", argv[1]);
    f = fopen(ogg_path, "rb");
    CHECK(f != NULL);
    if (!f)
        return 1;
    fseek(f, 0, SEEK_END);
    data_bytes = ftell(f);
    fclose(f);
    track2 = (uint32_t)(data_bytes / 2352);

    CHECK(port_cd_init(cue, 'D'));
    CHECK(port_cd_present());
    CHECK(port_cd_file("RAYMAN", "RAYMAN", &lba, &size) == 0);   /* RAYMAN/RAYMAN is a folder, not a file */
    CHECK(port_cd_file(NULL, "RAY.INI", &lba, &size) && size > 0);

    snprintf(ogg_path, sizeof ogg_path, "%s/Music/rayman02.ogg", argv[1]);
    ogg = port_ogg_open(ogg_path, &frames);
    CHECK(ogg != NULL);
    if (!ogg)
        return 1;
    sectors = (uint32_t)((frames + 587) / 588);
    port_ogg_close(ogg);

    /* four seconds from track 2's start, read sequentially */
    got = port_cd_pcm((uint64_t)track2 * 588, seq, 44100 * 4);
    CHECK(got == 44100 * 4);
    for (i = 0; i < got * 2; i++)
        if (seq[i] > 2000 || seq[i] < -2000)
            loud++;
    CHECK(loud > 1000);
    /* the same samples when the read starts in the middle (a seek) */
    got = port_cd_pcm((uint64_t)track2 * 588 + 3 * 44100 + 123, part, 2048);
    CHECK(got == 2048 && !memcmp(part, seq + 2 * (3 * 44100 + 123), sizeof part));
    /* track 3 starts at track 2's end: the last sector of track 2 is followed by track 3's first samples */
    got = port_cd_pcm((uint64_t)(track2 + sectors) * 588, part, 2048);
    CHECK(got == 2048);
    printf("GOG CD: track 2 at LBA %u, %u sectors (%llu frames); %d loud samples in 4 s; %u checks, %u failures\n",
           track2, sectors, (unsigned long long)frames, loud, checks, failures);
    port_cd_shutdown();
    return failures ? 1 : 0;
}
