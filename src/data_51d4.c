/* GOG v1.21 _DATA 2:051d4. Typed reconstruction; see NOTES.md.
 * Block split and dt_str_* names INVENTED. No literal pointer addresses. */
#include "game.h"

extern char dt_str_empty[];
extern char dt_str_michel_ancel[];
extern char dt_str_frederic_houde[];
extern char dt_str_alexandra_steible[];
extern char dt_str_serge_hascoet[];
extern char dt_str_bruno_bouvret[];
extern char dt_str_gunther_galipot[];
extern char dt_str_sacha_gentilhomme[];
extern char dt_str_michael_guez[];
extern char dt_str_christophe_thibaut[];
extern char dt_str_kamal_aitmihoub[];
extern char dt_str_david_gilson[];
extern char dt_str_sophie_esturgie[];
extern char dt_str_jacques_exertie[];
extern char dt_str_gregoire_pons[];
extern char dt_str_veronique_rondello[];
extern char dt_str_beatrice_sauterau[];
extern char dt_str_philippe_vindolet[];
extern char dt_str_christian_volckman[];
extern char dt_str_eric_pelatan[];
extern char dt_str_sylvaine_jenny[];
extern char dt_str_jean_christophe_alessandri[];
extern char dt_str_sophie_ancel[];
extern char dt_str_olivier_bechard[];
extern char dt_str_nicolas_bocquillon[];
extern char dt_str_jean_marc_geffroy[];
extern char dt_str_laurent_rettig[];
extern char dt_str_olivier_soleil[];
extern char dt_str_bertrand_helias[];
extern char dt_str_yann_le_tensorer[];
extern char dt_str_daniel_palix[];
extern char dt_str_cyril_cauchois[];
extern char dt_str_olivier_couvreur[];
extern char dt_str_antoine_dodens[];
extern char dt_str_antonio_ferreira[];
extern char dt_str_philippe_fuentes[];
extern char dt_str_benoit_germain[];
extern char dt_str_christophe_giraud[];
extern char dt_str_francois_mahieu[];
extern char dt_str_christian_mihalache[];
extern char dt_str_sebastien_morin[];
extern char dt_str_bogdan_radusi[];
extern char dt_str_daniel_raviart[];
extern char dt_str_olivier_rozier[];
extern char dt_str_jozef_skrabo[];
extern char dt_str_gilles_vanwalleghem[];
extern char dt_str_stephane_bellanger[];
extern char dt_str_didier_lord[];
extern char dt_str_olivier_amiaud[];
extern char dt_str_sylvain_brunet[];
extern char dt_str_nathalie_drouet[];
extern char dt_str_dominique_dumont[];
extern char dt_str_kamel_feunas[];
extern char dt_str_remy_gazel[];
extern char dt_str_didier_leglise[];
extern char dt_str_jean_marc_litchmann[];
extern char dt_str_frederic_louvre[];
extern char dt_str_guillaume_michel[];
extern char dt_str_olivier_mortier[];
extern char dt_str_frederic_prados[];
extern char dt_str_stephane_ronse[];
extern char dt_str_rene_de_wael[];
extern char dt_str_emmanuelle_cosso[];
extern char dt_str_mitsuyuki_bando[];
extern char dt_str_chris_benard[];
extern char dt_str_patrice_dozier[];
extern char dt_str_douglas_rand[];
extern char dt_str_steven_perkinson[];
extern char dt_str_christian_stonner[];
extern char dt_str_bruno_barone[];
extern char dt_str_thomas_belmont[];
extern char dt_str_christophe_cavelan[];
extern char dt_str_marc_gioan[];
extern char dt_str_benoit_macon[];
extern char dt_str_lionel_rico[];
extern char dt_str_emmanuel_texandier[];
extern char dt_str_dorian_thibaut[];
extern char dt_str_agnes_haegel[];
extern char dt_str_ludimedia_gerard_guillemot[];
extern char dt_str_ubi_studios_michel_guillemot[];
extern char dt_str_ubi_soft_yves_guillemot[];
extern char dt_str_rayman[];

/* INIT_CREDITS 1:5af91..5aff6: 12-byte copies, cmd byte at +9.
 * STRONG capacity: source ends at DemoRecord 2:05828; the destination
 * credits 2:3d140 ends at save_ray_copy 2:3d794. Both extents are 135 * 12.
 * Slots 115..134 are implicitly zero-initialised reserve records. */
credit_t credits_init[135] = {
    {dt_str_empty, 160, 110, 1, 4, 2, 0},
    {dt_str_empty, 160, 112, 2, 102, 1, 0},
    {dt_str_michel_ancel, 160, 0, 2, 0, 1, 0},
    {dt_str_frederic_houde, 160, 0, 2, 3, 1, 0},
    {dt_str_empty, 160, 113, 2, 102, 2, 0},
    {dt_str_alexandra_steible, 160, 0, 2, 3, 2, 0},
    {dt_str_empty, 160, 115, 2, 102, 5, 0},
    {dt_str_michel_ancel, 160, 0, 2, 0, 5, 0},
    {dt_str_serge_hascoet, 160, 0, 2, 0, 5, 0},
    {dt_str_empty, 160, 111, 2, 0, 5, 0},
    {dt_str_bruno_bouvret, 160, 0, 2, 0, 5, 0},
    {dt_str_gunther_galipot, 160, 0, 2, 0, 5, 0},
    {dt_str_sacha_gentilhomme, 160, 0, 2, 0, 5, 0},
    {dt_str_michael_guez, 160, 0, 2, 0, 5, 0},
    {dt_str_christophe_thibaut, 160, 0, 2, 3, 5, 0},
    {dt_str_empty, 160, 116, 2, 102, 0, 0},
    {dt_str_alexandra_steible, 160, 0, 2, 0, 0, 0},
    {dt_str_empty, 160, 111, 2, 0, 0, 0},
    {dt_str_kamal_aitmihoub, 160, 0, 2, 0, 0, 0},
    {dt_str_david_gilson, 160, 0, 2, 0, 0, 0},
    {dt_str_sophie_esturgie, 160, 0, 2, 0, 0, 0},
    {dt_str_jacques_exertie, 160, 0, 2, 0, 0, 0},
    {dt_str_gregoire_pons, 160, 0, 2, 0, 0, 0},
    {dt_str_veronique_rondello, 160, 0, 2, 0, 0, 0},
    {dt_str_beatrice_sauterau, 160, 0, 2, 0, 0, 0},
    {dt_str_philippe_vindolet, 160, 0, 2, 0, 0, 0},
    {dt_str_christian_volckman, 160, 0, 2, 3, 0, 0},
    {dt_str_empty, 160, 117, 2, 102, 1, 0},
    {dt_str_eric_pelatan, 160, 0, 2, 0, 1, 0},
    {dt_str_sylvaine_jenny, 160, 0, 2, 3, 1, 0},
    {dt_str_empty, 160, 114, 2, 102, 0, 0},
    {dt_str_jean_christophe_alessandri, 160, 0, 2, 0, 0, 0},
    {dt_str_sophie_ancel, 160, 0, 2, 0, 0, 0},
    {dt_str_olivier_bechard, 160, 0, 2, 0, 0, 0},
    {dt_str_nicolas_bocquillon, 160, 0, 2, 0, 0, 0},
    {dt_str_jean_marc_geffroy, 160, 0, 2, 0, 0, 0},
    {dt_str_laurent_rettig, 160, 0, 2, 0, 0, 0},
    {dt_str_olivier_soleil, 160, 0, 2, 3, 0, 0},
    {dt_str_empty, 160, 118, 2, 102, 5, 0},
    {dt_str_bertrand_helias, 160, 0, 2, 0, 5, 0},
    {dt_str_yann_le_tensorer, 160, 0, 2, 0, 5, 0},
    {dt_str_daniel_palix, 160, 0, 2, 0, 5, 0},
    {dt_str_empty, 160, 111, 2, 0, 5, 0},
    {dt_str_cyril_cauchois, 160, 0, 2, 0, 5, 0},
    {dt_str_olivier_couvreur, 160, 0, 2, 0, 5, 0},
    {dt_str_antoine_dodens, 160, 0, 2, 0, 5, 0},
    {dt_str_antonio_ferreira, 160, 0, 2, 0, 5, 0},
    {dt_str_philippe_fuentes, 160, 0, 2, 0, 5, 0},
    {dt_str_benoit_germain, 160, 0, 2, 0, 5, 0},
    {dt_str_christophe_giraud, 160, 0, 2, 0, 5, 0},
    {dt_str_frederic_houde, 160, 0, 2, 0, 5, 0},
    {dt_str_francois_mahieu, 160, 0, 2, 0, 5, 0},
    {dt_str_christian_mihalache, 160, 0, 2, 0, 5, 0},
    {dt_str_sebastien_morin, 160, 0, 2, 0, 5, 0},
    {dt_str_bogdan_radusi, 160, 0, 2, 0, 5, 0},
    {dt_str_daniel_raviart, 160, 0, 2, 0, 5, 0},
    {dt_str_olivier_rozier, 160, 0, 2, 0, 5, 0},
    {dt_str_jozef_skrabo, 160, 0, 2, 0, 5, 0},
    {dt_str_gilles_vanwalleghem, 160, 0, 2, 3, 5, 0},
    {dt_str_empty, 160, 119, 2, 102, 0, 0},
    {dt_str_stephane_bellanger, 160, 0, 2, 0, 0, 0},
    {dt_str_didier_lord, 160, 0, 2, 0, 0, 0},
    {dt_str_empty, 160, 111, 2, 0, 0, 0},
    {dt_str_olivier_amiaud, 160, 0, 2, 0, 0, 0},
    {dt_str_sylvain_brunet, 160, 0, 2, 0, 0, 0},
    {dt_str_nathalie_drouet, 160, 0, 2, 0, 0, 0},
    {dt_str_dominique_dumont, 160, 0, 2, 0, 0, 0},
    {dt_str_kamel_feunas, 160, 0, 2, 0, 0, 0},
    {dt_str_remy_gazel, 160, 0, 2, 0, 0, 0},
    {dt_str_didier_leglise, 160, 0, 2, 0, 0, 0},
    {dt_str_jean_marc_litchmann, 160, 0, 2, 0, 0, 0},
    {dt_str_frederic_louvre, 160, 0, 2, 0, 0, 0},
    {dt_str_guillaume_michel, 160, 0, 2, 0, 0, 0},
    {dt_str_olivier_mortier, 160, 0, 2, 0, 0, 0},
    {dt_str_frederic_prados, 160, 0, 2, 0, 0, 0},
    {dt_str_stephane_ronse, 160, 0, 2, 0, 0, 0},
    {dt_str_rene_de_wael, 160, 0, 2, 3, 0, 0},
    {dt_str_empty, 160, 120, 2, 102, 1, 0},
    {dt_str_emmanuelle_cosso, 160, 0, 2, 0, 1, 0},
    {dt_str_empty, 160, 111, 2, 0, 1, 0},
    {dt_str_mitsuyuki_bando, 160, 0, 2, 0, 1, 0},
    {dt_str_chris_benard, 160, 0, 2, 0, 1, 0},
    {dt_str_patrice_dozier, 160, 0, 2, 0, 1, 0},
    {dt_str_douglas_rand, 160, 0, 2, 0, 1, 0},
    {dt_str_steven_perkinson, 160, 0, 2, 0, 1, 0},
    {dt_str_christian_stonner, 160, 0, 2, 3, 1, 0},
    {dt_str_empty, 160, 121, 2, 102, 1, 0},
    {dt_str_serge_hascoet, 160, 0, 2, 0, 1, 0},
    {dt_str_empty, 160, 111, 2, 0, 1, 0},
    {dt_str_bruno_barone, 160, 0, 2, 0, 1, 0},
    {dt_str_thomas_belmont, 160, 0, 2, 0, 1, 0},
    {dt_str_nicolas_bocquillon, 160, 0, 2, 0, 1, 0},
    {dt_str_bruno_bouvret, 160, 0, 2, 0, 1, 0},
    {dt_str_christophe_cavelan, 160, 0, 2, 0, 1, 0},
    {dt_str_gunther_galipot, 160, 0, 2, 0, 1, 0},
    {dt_str_sacha_gentilhomme, 160, 0, 2, 0, 1, 0},
    {dt_str_marc_gioan, 160, 0, 2, 0, 1, 0},
    {dt_str_michael_guez, 160, 0, 2, 0, 1, 0},
    {dt_str_benoit_macon, 160, 0, 2, 0, 1, 0},
    {dt_str_lionel_rico, 160, 0, 2, 0, 1, 0},
    {dt_str_emmanuel_texandier, 160, 0, 2, 0, 1, 0},
    {dt_str_christophe_thibaut, 160, 0, 2, 0, 1, 0},
    {dt_str_dorian_thibaut, 160, 0, 2, 3, 1, 0},
    {dt_str_empty, 160, 122, 2, 102, 2, 0},
    {dt_str_agnes_haegel, 160, 0, 2, 3, 2, 0},
    {dt_str_empty, 160, 123, 2, 102, 5, 0},
    {dt_str_ludimedia_gerard_guillemot, 160, 0, 2, 2, 5, 0},
    {dt_str_empty, 160, 124, 2, 102, 5, 0},
    {dt_str_ubi_studios_michel_guillemot, 160, 0, 2, 2, 5, 0},
    {dt_str_empty, 160, 125, 2, 102, 5, 0},
    {dt_str_ubi_soft_yves_guillemot, 160, 0, 2, 4, 5, 0},
    {dt_str_empty, 160, 126, 2, 0, 1, 0},
    {dt_str_empty, 160, 127, 2, 6, 1, 0},
    {dt_str_empty, 160, 128, 1, 0, 1, 0},
    {dt_str_rayman, 200, 0, 2, 255, 2, 0},
};
