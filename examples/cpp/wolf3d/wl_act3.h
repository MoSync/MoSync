#ifndef __WL_ACT3_H__
#define __WL_ACT3_H__

typedef enum {
	s_none,
	
	s_boom1,
	s_boom2,
	s_boom3,

	s_rocket,

	s_smoke1,
	s_smoke2,
	s_smoke3,
	s_smoke4,

	s_grdstand,

	s_grdpath1,
	s_grdpath1s,
	s_grdpath2,
	s_grdpath3,
	s_grdpath3s,
	s_grdpath4,

	s_grdpain,
	s_grdpain1,

	s_grdshoot1,
	s_grdshoot2,
	s_grdshoot3,

	s_grdchase1,
	s_grdchase1s,
	s_grdchase2,
	s_grdchase3,
	s_grdchase3s,
	s_grdchase4,

	s_grddie1,
	s_grddie2,
	s_grddie3,
	s_grddie4,


	s_dogpath1,
	s_dogpath1s,
	s_dogpath2,
	s_dogpath3,
	s_dogpath3s,
	s_dogpath4,

	s_dogjump1,
	s_dogjump2,
	s_dogjump3,
	s_dogjump4,
	s_dogjump5,

	s_dogchase1,
	s_dogchase1s,
	s_dogchase2,
	s_dogchase3,
	s_dogchase3s,
	s_dogchase4,

	s_dogdie1,
	s_dogdie2,
	s_dogdie3,
	s_dogdead,

//
// officers
//

	s_ofcstand,

	s_ofcpath1,
	s_ofcpath1s,
	s_ofcpath2,
	s_ofcpath3,
	s_ofcpath3s,
	s_ofcpath4,

	s_ofcpain,
	s_ofcpain1,

	s_ofcshoot1,
	s_ofcshoot2,
	s_ofcshoot3,

	s_ofcchase1,
	s_ofcchase1s,
	s_ofcchase2,
	s_ofcchase3,
	s_ofcchase3s,
	s_ofcchase4,

	s_ofcdie1,
	s_ofcdie2,
	s_ofcdie3,
	s_ofcdie4,
	s_ofcdie5,



//
// mutant
//

	s_mutstand,

	s_mutpath1,
	s_mutpath1s,
	s_mutpath2,
	s_mutpath3,
	s_mutpath3s,
	s_mutpath4,

	s_mutpain,
	s_mutpain1,

	s_mutshoot1,
	s_mutshoot2,
	s_mutshoot3,
	s_mutshoot4,

	s_mutchase1,
	s_mutchase1s,
	s_mutchase2,
	s_mutchase3,
	s_mutchase3s,
	s_mutchase4,

	s_mutdie1,
	s_mutdie2,
	s_mutdie3,
	s_mutdie4,
	s_mutdie5,


//
// SS
//

	s_ssstand,

	s_sspath1,
	s_sspath1s,
	s_sspath2,
	s_sspath3,
	s_sspath3s,
	s_sspath4,

	s_sspain,
	s_sspain1,

	s_ssshoot1,
	s_ssshoot2,
	s_ssshoot3,
	s_ssshoot4,
	s_ssshoot5,
	s_ssshoot6,
	s_ssshoot7,
	s_ssshoot8,
	s_ssshoot9,

	s_sschase1,
	s_sschase1s,
	s_sschase2,
	s_sschase3,
	s_sschase3s,
	s_sschase4,

	s_ssdie1,
	s_ssdie2,
	s_ssdie3,
	s_ssdie4,



#ifdef SPEAR

	s_hrocket,
	s_hsmoke1,
	s_hsmoke2,
	s_hsmoke3,
	s_hsmoke4,

	s_hboom1,
	s_hboom2,
	s_hboom3,

//
// trans
//
	s_transstand,

	s_transchase1,
	s_transchase1s,
	s_transchase2,
	s_transchase3,
	s_transchase3s,
	s_transchase4,

	s_transdie0,
	s_transdie01,
	s_transdie1,
	s_transdie2,
	s_transdie3,
	s_transdie4,

	s_transshoot1,
	s_transshoot2,
	s_transshoot3,
	s_transshoot4,
	s_transshoot5,
	s_transshoot6,
	s_transshoot7,
	s_transshoot8,

	s_uberstand,

	s_uberchase1,
	s_uberchase1s,
	s_uberchase2,
	s_uberchase3,
	s_uberchase3s,
	s_uberchase4,

	s_uberdie0,
	s_uberdie01,
	s_uberdie1,
	s_uberdie2,
	s_uberdie3,
	s_uberdie4,
	s_uberdie5,

	s_ubershoot1,
	s_ubershoot2,
	s_ubershoot3,
	s_ubershoot4,
	s_ubershoot5,
	s_ubershoot6,
	s_ubershoot7,

//
// will
//
	s_willstand,

	s_willchase1,
	s_willchase1s,
	s_willchase2,
	s_willchase3,
	s_willchase3s,
	s_willchase4,

	s_willdie1,
	s_willdie2,
	s_willdie3,
	s_willdie4,
	s_willdie5,
	s_willdie6,

	s_willshoot1,
	s_willshoot2,
	s_willshoot3,
	s_willshoot4,
	s_willshoot5,
	s_willshoot6,

//
// death
//
	s_deathstand,

	s_deathchase1,
	s_deathchase1s,
	s_deathchase2,
	s_deathchase3,
	s_deathchase3s,
	s_deathchase4,

	s_deathdie1,
	s_deathdie2,
	s_deathdie3,
	s_deathdie4,
	s_deathdie5,
	s_deathdie6,
	s_deathdie7,
	s_deathdie8,
	s_deathdie9,

	s_deathshoot1,
	s_deathshoot2,
	s_deathshoot3,
	s_deathshoot4,
	s_deathshoot5,

//
// angel
//

	s_angelstand,

	s_angelchase1,
	s_angelchase1s,
	s_angelchase2,
	s_angelchase3,
	s_angelchase3s,
	s_angelchase4,

	s_angeldie1,
	s_angeldie11,
	s_angeldie2,
	s_angeldie3,
	s_angeldie4,
	s_angeldie5,
	s_angeldie6,
	s_angeldie7,
	s_angeldie8,
	s_angeldie9,

	s_angelshoot1,
	s_angelshoot2,
	s_angelshoot3,

	s_angeltired,
	s_angeltired2,
	s_angeltired3,
	s_angeltired4,
	s_angeltired5,
	s_angeltired6,
	s_angeltired7,

	s_spark1,
	s_spark2,
	s_spark3,
	s_spark4,


	s_spectrewait1,
	s_spectrewait2,
	s_spectrewait3,
	s_spectrewait4,

	s_spectrechase1,
	s_spectrechase2,
	s_spectrechase3,
	s_spectrechase4,

	s_spectredie1,
	s_spectredie2,
	s_spectredie3,
	s_spectredie4,

	s_spectrewake,

#endif

#ifndef SPEAR

	s_blinkychase1,
	s_blinkychase2,
	s_inkychase1,
	s_inkychase2,
	s_pinkychase1,
	s_pinkychase2,
	s_clydechase1,
	s_clydechase2,

//
// hans
//
	s_bossstand,

	s_bosschase1,
	s_bosschase1s,
	s_bosschase2,
	s_bosschase3,
	s_bosschase3s,
	s_bosschase4,

	s_bossdie1,
	s_bossdie2,
	s_bossdie3,
	s_bossdie4,

	s_bossshoot1,
	s_bossshoot2,
	s_bossshoot3,
	s_bossshoot4,
	s_bossshoot5,
	s_bossshoot6,
	s_bossshoot7,
	s_bossshoot8,

//
// gretel
//
	s_gretelstand,

	s_gretelchase1,
	s_gretelchase1s,
	s_gretelchase2,
	s_gretelchase3,
	s_gretelchase3s,
	s_gretelchase4,

	s_greteldie1,
	s_greteldie2,
	s_greteldie3,
	s_greteldie4,

	s_gretelshoot1,
	s_gretelshoot2,
	s_gretelshoot3,
	s_gretelshoot4,
	s_gretelshoot5,
	s_gretelshoot6,
	s_gretelshoot7,
	s_gretelshoot8,

//
// schabb
//
	s_schabbstand,

	s_schabbchase1,
	s_schabbchase1s,
	s_schabbchase2,
	s_schabbchase3,
	s_schabbchase3s,
	s_schabbchase4,

	s_schabbdeathcam,

	s_schabbdie1,
	s_schabbdie2,
	s_schabbdie3,
	s_schabbdie4,
	s_schabbdie5,
	s_schabbdie6,

	s_schabbshoot1,
	s_schabbshoot2,

	s_needle1,
	s_needle2,
	s_needle3,
	s_needle4,


//
// gift
//
	s_giftstand,

	s_giftchase1,
	s_giftchase1s,
	s_giftchase2,
	s_giftchase3,
	s_giftchase3s,
	s_giftchase4,

	s_giftdeathcam,

	s_giftdie1,
	s_giftdie2,
	s_giftdie3,
	s_giftdie4,
	s_giftdie5,
	s_giftdie6,

	s_giftshoot1,
	s_giftshoot2,

//
// fat
//
	s_fatstand,

	s_fatchase1,
	s_fatchase1s,
	s_fatchase2,
	s_fatchase3,
	s_fatchase3s,
	s_fatchase4,

	s_fatdeathcam,

	s_fatdie1,
	s_fatdie2,
	s_fatdie3,
	s_fatdie4,
	s_fatdie5,
	s_fatdie6,

	s_fatshoot1,
	s_fatshoot2,
	s_fatshoot3,
	s_fatshoot4,
	s_fatshoot5,
	s_fatshoot6,


//
// fake
//
	s_fakestand,

	s_fakechase1,
	s_fakechase1s,
	s_fakechase2,
	s_fakechase3,
	s_fakechase3s,
	s_fakechase4,

	s_fakedie1,
	s_fakedie2,
	s_fakedie3,
	s_fakedie4,
	s_fakedie5,
	s_fakedie6,

	s_fakeshoot1,
	s_fakeshoot2,
	s_fakeshoot3,
	s_fakeshoot4,
	s_fakeshoot5,
	s_fakeshoot6,
	s_fakeshoot7,
	s_fakeshoot8,
	s_fakeshoot9,

	s_fire1,
	s_fire2,


//
// hitler
//

	s_mechastand,

	s_mechachase1,
	s_mechachase1s,
	s_mechachase2,
	s_mechachase3,
	s_mechachase3s,
	s_mechachase4,

	s_mechadie1,
	s_mechadie2,
	s_mechadie3,
	s_mechadie4,

	s_mechashoot1,
	s_mechashoot2,
	s_mechashoot3,
	s_mechashoot4,
	s_mechashoot5,
	s_mechashoot6,


	s_hitlerchase1,
	s_hitlerchase1s,
	s_hitlerchase2,
	s_hitlerchase3,
	s_hitlerchase3s,
	s_hitlerchase4,

	s_hitlerdeathcam,

	s_hitlerdie1,
	s_hitlerdie2,
	s_hitlerdie3,
	s_hitlerdie4,
	s_hitlerdie5,
	s_hitlerdie6,
	s_hitlerdie7,
	s_hitlerdie8,
	s_hitlerdie9,
	s_hitlerdie10,

	s_hitlershoot1,
	s_hitlershoot2,
	s_hitlershoot3,
	s_hitlershoot4,
	s_hitlershoot5,
	s_hitlershoot6,

//
// BJ victory
//

	s_bjrun1,
	s_bjrun1s,
	s_bjrun2,
	s_bjrun3,
	s_bjrun3s,
	s_bjrun4,

	s_bjjump1,
	s_bjjump2,
	s_bjjump3,
	s_bjjump4,

	s_deathcam,

#endif

	s_player,
	s_attack,

	MAXSTATES
} stateenum;

extern statetype gamestates[MAXSTATES];

#endif
