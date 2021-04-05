
/*==============================================================

	必殺

================================================================*/
#include "character.h"


/*----------------------------------------------------------------
	ポイ (Aルート)
------------------------------------------------------------------*/

//お茶
void CCharacter::act_poi_ocya()
{
	if(ccounter==0)PlayMySound(wave_poi_1a);
	if(ccounter<3)cell(CELL_POI1);
	else if(ccounter<4)cell(CELL_POI2);
	else if(ccounter<20)cell(CELL_POI3);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==3 && pbul_ocya)
	{
		if(pbul_ocya->isReady())
		pbul_ocya->Go(pdat->muki,pdat->x+zurex(30),-70,6,-12);
	}
}

//ジエン
void CCharacter::act_poi_jien()
{
	if(ccounter==0)PlayMySound(wave_poi_1b);
	if(ccounter<3)cell(CELL_POI1);
	else if(ccounter<4)cell(CELL_POI2);
	else if(ccounter<20)cell(CELL_POI3);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==3 && pbul_jien)
	{
		if(pbul_jien->isReady())
		pbul_jien->Go(pdat->muki,pdat->x+zurex(30),-70,8,-3);
	}
}

//アロエリーナ
void CCharacter::act_poi_aroe()
{
	if(ccounter==0)PlayMySound(wave_poi_1c);
	if(ccounter<3)cell(CELL_POI1);
	else if(ccounter<4)cell(CELL_POI2);
	else if(ccounter<20)cell(CELL_POI3);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==3 && pbul_aroe)
	{
		if(pbul_aroe->isReady())
			pbul_aroe->Go(pdat->muki,pdat->x+zurex(30),-70,2,-28);
	}
}

//モウコネエヨ
void CCharacter::act_poi_moukoneeyo()
{
	if(ccounter==0)PlayMySound(wave_poi_1d);
	if(ccounter<3)cell(CELL_POI1);
	else if(ccounter<4)cell(CELL_POI2);
	else if(ccounter<20)cell(CELL_POI3);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==3)
	{
		if(pbul_moukoneeyo->isReady())
		{
			pbul_moukoneeyo->Go(pdat->muki);
			pbul_comeon->Go(pdat->muki,pdat->x+zurex(60),-100,0,0);
		}
	}
}

//ミサイル（水平）
void CCharacter::act_poi_missile_h()
{
	if(ccounter==0)PlayMySound(wave_poi_1e);
	if(ccounter<10)cell(CELL_HLUNCH1);
	else if(ccounter<11)cell(CELL_HLUNCH2);//1
	else if(ccounter<20)cell(CELL_HLUNCH1);
	else if(ccounter<21)cell(CELL_HLUNCH2);//2
	else if(ccounter<30)cell(CELL_HLUNCH1);
	else if(ccounter<31)cell(CELL_HLUNCH2);//3
	else if(ccounter<40)cell(CELL_HLUNCH1);
	else if(ccounter<41)cell(CELL_HLUNCH2);//4
	else if(ccounter<60)cell(CELL_HLUNCH1);
	else ChangeAction( ACTID_NEUTRAL );

	if( ccounter==10 || ccounter==20 || ccounter==30 || ccounter==40 )
	{
		for(UINT i=0;i<MISSILE_NUM;i++)
		{
			if(pbul_miss[i])
			{
				if(pbul_miss[i]->isReady())
				{
					pbul_miss[i]->Go( pdat->muki,
									  pdat->x + zurex(100),
									  -70,
									  15 * (pdat->muki ? -1 : 1),
									  0);
					break;
				}
			}
		}
	}
}

//ミサイル一斉発射
void CCharacter::act_poi_missileSP()
{
	if(ccounter==0)PlayMySound(wave_poi_1f);
	if(ccounter < 50)
	{
		//水平x4
		act_poi_missile_h();
	}
	else if(ccounter < 50+40)
	{
		//垂直x4
		act_poi_missile_v2( ccounter%10 );
	}
	else if(ccounter < 50+40+50)
	{
		//水平x4
		pdat->counter -= 50+40;
		act_poi_missile_h();
		pdat->counter += 50+40;
	}
	else ChangeAction( ACTID_NEUTRAL );
}


/*----------------------------------------------------------------
	ポイ (Bルート)
------------------------------------------------------------------*/

//ハンドグレネード
void CCharacter::act_poi_handg()
{
	if(ccounter==0)PlayMySound(wave_poi_2a);
	if(ccounter<3)cell(CELL_POI1);
	else if(ccounter<4)cell(CELL_POI2);
	else if(ccounter<20)cell(CELL_POI3);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==3)
	{
		if(pbul_handg->isReady())
			pbul_handg->Go(pdat->muki,pdat->x+zurex(30),-70,4.5,-12);
	}
}

//クレイモア
void CCharacter::act_poi_craymore()
{
	if(ccounter==0)PlayMySound(wave_poi_2b);
	if(ccounter<3)cell(CELL_POI1);
	else if(ccounter<4)cell(CELL_POI2);
	else if(ccounter<20)cell(CELL_POI3);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==3)
	{
		if(pbul_craymore->isReady())
			pbul_craymore->Go(pdat->muki,pdat->x+zurex(30),-70,3.5,-10);
	}
}

//ジャック・ランタン
void CCharacter::act_poi_lantern()
{
	if(ccounter==0)PlayMySound(wave_poi_2c);
	if(ccounter<3)cell(CELL_POI1);
	else if(ccounter<4)cell(CELL_POI2);
	else if(ccounter<20)cell(CELL_POI3);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==3)
	{
		if(pbul_lantern->isReady())
		{
			pbul_lantern->Go(pdat->muki,0,-10000,0,0);
			pbul_comeon->Go(pdat->muki,pdat->x+zurex(60),-100,0,0);
		}
	}
}

//火炎放射
void CCharacter::act_poi_firethrow()
{
	if(ccounter==0)PlayMySound(wave_poi_2d);
	const UINT duration = DUR_FIRETHROW;

	if(ccounter<2)cell(CELL_FIRETHROW1);
	else if(ccounter<4)cell(CELL_FIRETHROW2);
	else if(ccounter<6)cell(CELL_FIRETHROW3);
	else if(ccounter<8)cell(CELL_FIRETHROW4);
	else if(ccounter<10)cell(CELL_FIRETHROW5);
	else if(ccounter<10+duration)cell(CELL_FIRETHROW6);
	else if(ccounter<10+duration+2)cell(CELL_FIRETHROW5);
	else if(ccounter<10+duration+4)cell(CELL_FIRETHROW4);
	else if(ccounter<10+duration+8)cell(CELL_FIRETHROW3);
	else if(ccounter<10+duration+10)cell(CELL_FIRETHROW2);
	else if(ccounter<10+duration+12)cell(CELL_FIRETHROW1);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==11)
	{
		pbul_firethrower->Go(pdat->muki,pdat->x+zurex(130),-85,0);
	}
}

//ヘルファイア
void CCharacter::act_poi_hellfire()
{
	if(ccounter==0)PlayMySound(wave_poi_2e);
	const UINT duration = DUR_FIRETHROW;

	if(ccounter<2)cell(CELL_FIRETHROW1);
	else if(ccounter<4)cell(CELL_FIRETHROW2);
	else if(ccounter<6)cell(CELL_FIRETHROW3);
	else if(ccounter<8)cell(CELL_FIRETHROW4);
	else if(ccounter<10)cell(CELL_FIRETHROW5);
	else if(ccounter<10+duration)cell(CELL_FIRETHROW6);
	else if(ccounter<10+duration+2)cell(CELL_FIRETHROW5);
	else if(ccounter<10+duration+4)cell(CELL_FIRETHROW4);
	else if(ccounter<10+duration+8)cell(CELL_FIRETHROW3);
	else if(ccounter<10+duration+10)cell(CELL_FIRETHROW2);
	else if(ccounter<10+duration+12)cell(CELL_FIRETHROW1);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==3)
	{
		if(pbul_lantern->isReady())
		{
			pbul_lantern->Summon();
			pbul_comeon->Go(pdat->muki,pdat->x+zurex(60),-100,0,0);
		}
	}

	if(ccounter==11)
	{
		pbul_firethrower->Go(pdat->muki,pdat->x+zurex(130),-85,0);
	}
}


/*----------------------------------------------------------------
	ポイ (Cルート)
------------------------------------------------------------------*/

//ショットガン
void CCharacter::act_poi_shot()
{
	if(ccounter==0)PlayMySound(wave_poi_3a);
	const UINT suki = 20;
	const UINT tame = 10;

	if(ccounter<2)cell(CELL_SHOT1);
	else if(ccounter<4)cell(CELL_SHOT2);
	else if(ccounter<6)cell(CELL_SHOT3);
	else if(ccounter<8)cell(CELL_SHOT4);
	else if(ccounter<10)cell(CELL_SHOT5);
	else if(ccounter<12)cell(CELL_SHOT6);
	else if(ccounter<14)cell(CELL_SHOT7);
	else if(ccounter<20)cell(CELL_SHOT8);//shot!!
	else if(ccounter<22)cell(CELL_SHOT9);
	else if(ccounter<24)cell(CELL_SHOT10);
	else if(ccounter<26)cell(CELL_SHOT11);
	else if(ccounter<28+suki)cell(CELL_SHOT12);
	else if(ccounter<30+suki+2)cell(CELL_SHOT7);
	else if(ccounter<30+suki+4)cell(CELL_SHOT6);
	else if(ccounter<30+suki+6)cell(CELL_SHOT5);
	else if(ccounter<30+suki+8)cell(CELL_SHOT4);
	else if(ccounter<30+suki+10)cell(CELL_SHOT3);
	else if(ccounter<30+suki+12)cell(CELL_SHOT2);
	else if(ccounter<30+suki+14)cell(CELL_SHOT1);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==20)
	{
		pbul_shotgun->Go(pdat->muki,(int)(pdat->x+zurex(130)),-60,0,0);
	}
}

//ライフル(?)
void CCharacter::act_poi_rifle()
{
	if(ccounter==0)PlayMySound(wave_poi_3b);
	const UINT kamae = RIFLE_TIMEOUT;
	const UINT suki = 20;

	if(ccounter<2)cell(CELL_RIFLE1);
	else if(ccounter<4)cell(CELL_RIFLE2);
	else if(ccounter<6)cell(CELL_RIFLE3);
	else if(ccounter<8)cell(CELL_RIFLE4);
	else if(ccounter<10+kamae)cell(CELL_RIFLE5);//構え , shot!!
	else if(ccounter<12+kamae+2)cell(CELL_RIFLE6);
	else if(ccounter<12+kamae+4)cell(CELL_RIFLE7);
	else if(ccounter<12+kamae+6)cell(CELL_RIFLE8);
	else if(ccounter<12+kamae+8+suki)cell(CELL_RIFLE5);
	else if(ccounter<12+kamae+6+suki+2)cell(CELL_RIFLE4);
	else if(ccounter<12+kamae+6+suki+4)cell(CELL_RIFLE3);
	else if(ccounter<12+kamae+6+suki+6)cell(CELL_RIFLE2);
	else if(ccounter<12+kamae+6+suki+8)cell(CELL_RIFLE1);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==0){
		pbul_rifle->SetKeyIndex( (pdat->objtype&GOBJFLG_COMPUTER) ? -1 : keyinput );
		pbul_rifle->Go(pdat->muki,pdat->x+zurex(160), -80,0);
	}
}

//無反動砲(？)
void CCharacter::act_poi_rocket()
{
	if(ccounter==0)PlayMySound(wave_poi_3c);
	const UINT tame = 10;
	const UINT dur = 60;

	if(ccounter<2)cell(CELL_BAZOOKA1);
	else if(ccounter<4)cell(CELL_BAZOOKA2);
	else if(ccounter<6)cell(CELL_BAZOOKA3);
	else if(ccounter<8)cell(CELL_BAZOOKA4);
	else if(ccounter<10)cell(CELL_BAZOOKA5);
	else if(ccounter<12)cell(CELL_BAZOOKA6);
	else if(ccounter<14+tame)cell(CELL_BAZOOKA7);
	else if(ccounter<14+tame+dur+2)cell(CELL_BAZOOKA8);
	else if(ccounter<14+tame+dur+4)cell(CELL_BAZOOKA9);
	else if(ccounter<14+tame+dur+6)cell(CELL_BAZOOKA10);
	else if(ccounter<14+tame+dur+8)cell(CELL_BAZOOKA11);
	else if(ccounter<14+tame+dur+10)cell(CELL_BAZOOKA12);
	else if(ccounter<14+tame+dur+12)cell(CELL_BAZOOKA13);
	else if(ccounter<14+tame+dur+14)cell(CELL_BAZOOKA14);
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==14+tame)
	{
		pbul_rocket->Go(pdat->muki,pdat->x+zurex(170), -83,0,0);
	}
}


//ミサイル（垂直）
void CCharacter::act_poi_missile_v()
{
	act_poi_missile_v2( ccounter%15 );
	if(ccounter > 15*4)ChangeAction(ACTID_NEUTRAL);
}

void CCharacter::act_poi_missile_v2(UINT counter)
{
	if(ccounter==0)PlayMySound(wave_poi_3d);
	if(counter<2)cell(CELL_VLUNCH1);
	else if(counter< 4)cell(CELL_VLUNCH2);
	else if(counter< 6)cell(CELL_VLUNCH3);
	else if(counter< 8)cell(CELL_VLUNCH4);
	else if(counter<10)cell(CELL_VLUNCH5);
	else cell(CELL_VLUNCH6);

	if(counter==2)
	{
		for(UINT i=0;i<MISSILE_NUM;i++)
		{
			if(pbul_miss[i])
			{
				if(pbul_miss[i]->isReady())
				{
					pbul_miss[i]->Go( pdat->muki,
									  pdat->x + zurex(-20),
									  -85,
									  0,
									  -15);
					break;
				}
			}
		}
	}
}

//http レーザー
void CCharacter::act_poi_httplaser()
{
	if(ccounter==0)PlayMySound(wave_poi_3e);
	UINT spd = 2;
	UINT dur = 60;

	if(ccounter<spd*1)cell( CELL_LASER1 );
	else if(ccounter<spd*2)cell( CELL_LASER2 );
	else if(ccounter<spd*3)cell( CELL_LASER3 );
	else if(ccounter<spd*4)cell( CELL_LASER4 );
	else if(ccounter<spd*5)cell( CELL_LASER5 );
	else if(ccounter<spd*6)cell( CELL_LASER6 );
	else if(ccounter<spd*7+10)cell( CELL_LASER7 );
	else if(ccounter<spd*8+dur)cell( CELL_LASER8 );
	else if(ccounter<spd*8+dur+spd*1)cell( CELL_LASER7 );
	else if(ccounter<spd*8+dur+spd*2)cell( CELL_LASER6 );
	else if(ccounter<spd*8+dur+spd*3)cell( CELL_LASER5 );
	else if(ccounter<spd*8+dur+spd*4)cell( CELL_LASER4 );
	else if(ccounter<spd*8+dur+spd*5)cell( CELL_LASER3 );
	else if(ccounter<spd*8+dur+spd*6)cell( CELL_LASER2 );
	else if(ccounter<spd*8+dur+spd*7)cell( CELL_LASER1 );
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==spd*7+10)
	{
		pbul_httplaser->Go(pdat->muki, pdat->x+zurex(212), -70,0);
	}
}

//イレイサー99
void CCharacter::act_poi_eraser99()
{
	UINT spd = 2;
	UINT dur = 300;

	if(ccounter==0)PlayMySound(wave_poi_3f1);
	if(ccounter==35*spd)PlayMySound(wave_poi_3f2);
	if(ccounter==35*spd+150)PlayMySound(wave_poi_3f3);

	if(ccounter < 35*spd)
	{
		cell( CELL_ERASER1 + (ccounter/spd)*2 );
	}
	else if(ccounter < 35*spd + dur)
	{
		if(ccounter < 35*spd + 150)cell( CELL_ERASER36 );
		else if(ccounter < 35*spd + 200)	
		{
			if(ccounter%2)	cell( CELL_ERASER36  );
			else			cell( CELL_ERASER36b );
			pdat->vx = -6;
		}
		else
		{
			cell( CELL_ERASER37 );
			movex(pdat->vx);
			pdat->vx += 0.2f;
			if(pdat->vx>0)pdat->vx = 0;
		}
	}
	else if(ccounter < 35*spd + dur + 35*spd)
	{
		UINT counter = ccounter - 35*spd - dur;
		cell( CELL_ERASER36 - (counter/spd)*2);
	}
	else
	{
		ChangeAction( ACTID_NEUTRAL );
	}

	if(ccounter==35*spd)
	{
		pbul_eraser99->Go(pdat->muki, pdat->x+zurex(340),-85,0,0);
	}
}




/*----------------------------------------------------------------
	もやもや
------------------------------------------------------------------*/
void CCharacter::act_moyamoya()
{
	if(ccounter==0)PlayMySound(wave_are_kore);
	if(ccounter==0)
	{
		if(pbul_moya)
		{
			pbul_moya->Go(pdat->muki ,pdat->x+zurex(75),-140);
		}
	}

	cell( CELL_GASAGOZO1 + (ccounter/4)%4 );
}

void CCharacter::act_moyamoya1()
{
	if(pdat->counter==0)pbul_moya = pbul_moyaA;
	act_moyamoya();
}
void CCharacter::act_moyamoya2()
{
	if(pdat->counter==0)pbul_moya = pbul_moyaB;
	act_moyamoya();
}
void CCharacter::act_moyamoya3()
{
	if(pdat->counter==0)pbul_moya = pbul_moyaC;
	act_moyamoya();
}

/*----------------------------------------------------------------
	ブラッディー・ダッコ
------------------------------------------------------------------*/
void CCharacter::act_dakko()
{
	if(ccounter==0)PlayMySound(wave_dakko);
	if(ccounter==0)
	{
		pdat->vx = 30.0;
		pbul_cutin->Go(pdat->muki,pdat->x+zurex(100),-80,0,0);
		AddEffect( EFCTID_CYOHI,(int)pdat->x, -70,0);
		AddPowerGauge(-1.0);
	}
	else if(ccounter==1)
	{
		StartAttack( ATTACKINFO_DAKKO );
	}

	pdat->vx-=2.5;
	if(pdat->vx<1.5f)pdat->vx=1.5f;
	movex(pdat->vx);
	
	pdat->kougeki = (pdat->vx>10.0f && ccounter!=0) ? TRUE : FALSE ;

	cell( CELL_DAKKO );
	if(ccounter>60){
		ChangeAction( ACTID_NEUTRAL );
	}
}

void CCharacter::act_dakko_back()
{
	if(pdat->counter == 0)
	{
		cell( CELL_DAKKO );
		pdat->color = 0xFF000000;

		pdat->vy=-8;
		pdat->vx=-4;

		RemoveProperty( GOBJFLG_DONOTSTOP );

		return;
	}
	else if(pdat->counter==1)
	{
		pdat->color = 0xFFFFFFFF;
		GOBJECT* eobj = GetInfo( m_dakko_eid );
		if(eobj)eobj->color = 0xFFFFFFFF;
	}

	pdat->cnow = CELL_DASHB1;
	JiyuuRakka(1,TRUE);
}


