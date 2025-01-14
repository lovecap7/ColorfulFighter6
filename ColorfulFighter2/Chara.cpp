#include "Chara.h"
#include "Input.h"
#include "Player.h"
#include "Bullet.h"
#include <cassert>
namespace
{
	//硬直フレームが0の技はDownにする
	constexpr int kDown = 0;

	//全体フレーム
	constexpr int kAllFramePunchLight = 14;//弱パンチ
	constexpr int kStartFramePunchLight = 4;//弱パンチ発生
	constexpr int kFinishFramePunchLight = 6;//弱パンチ持続終了

	constexpr int kAllFramePunchHigh = 22;//強パンチ
	constexpr int kStartFramePunchHigh = 13;//強パンチ発生

	constexpr int  kAllFrameKickLight = 18;//弱キック
	constexpr int  kStartFrameKickLight = 5;//弱キック発生

	constexpr int  kAllFrameKickHigh = 33;//強キック
	constexpr int  kStartFrameKickHigh = 12;//強キック発生

	constexpr int kAllFramePunchLightSquat = 8;//しゃがみ弱パンチ
	constexpr int kStartFramePunchLightSquat = 4;//しゃがみ弱パンチ発生

	constexpr int kAllFramePunchHighSquat = 26;//しゃがみ強パンチ
	constexpr int kStartFramePunchHighSquat = 9;//しゃがみ強パンチ発生

	constexpr int kAllFrameKickLightSquat = 14;//しゃがみ弱キック
	constexpr int kStartFrameKickLightSquat = 5;//しゃがみ弱キック発生

	constexpr int kAllFrameKickHighSquat = 14;//しゃがみ強キック
	constexpr int kStartFrameKickHighSquat = 9;//しゃがみ強キック

	//波動拳
	constexpr int kAllFrameCommand1 = 48;
	constexpr int kStartFrameCommand1Light = 16;//弱波動拳発生
	constexpr int kStartFrameCommand1High = 12;//強波動拳発生
	constexpr float kHadouSpeedLight = 15.0f;//波動拳弱のスピード
	constexpr float kHadouSpeedHigh = 25.0f;//波動拳強のスピード

	//昇竜拳の全体フレーム
	constexpr int kAllFrameCommand2 = 33;
	constexpr int kStartFrameCommand2 = 5;

	//スパイラルアローの全体フレーム
	constexpr int kAllFrameCommand4Light = 42;
	constexpr int kStartFrameCommand4Light = 9;
	constexpr int kAllFrameCommand4High = 49;
	constexpr int kStartFrameCommand4High = 15;

	//ソニックブーム
	constexpr int kAllFrameCommand5 = 40;
	constexpr int kStartFrameCommand5 = 10;//発生
	constexpr float kSonicSpeedLight = 15.0f;//ソニックブーム弱のスピード
	constexpr float kSonicSpeedHigh = 25.0f;//ソニックブーム強のスピード
	
	//闇払い
	constexpr int kAllFrameCommand6Light = 46;
	constexpr int kAllFrameCommand6High = 52;
	constexpr int kStartFrameCommand6Light = 16;//発生
	constexpr int kStartFrameCommand6High = 13;//発生
	constexpr float kWaveSpeedLight = 20.0f;//闇払い弱のスピード
	constexpr float kWaveSpeedHigh = 30.0f;//闇払い強のスピード

	//バーチカルローリングの全体フレーム
	constexpr int kAllFrameCommand7 = 48;
	constexpr int kStartFrameCommand7 = 8;

	//竜巻旋風脚
	constexpr int kAllFrameCommand8Light = 48;
	constexpr int kAllFrameCommand8High = 61;
	
}

Chara::Chara(int* index) :
	//モーション
	m_velocity(),
	m_handle(-1),//画像
	m_animNum(0),//アニメーションの数
	m_oneAnimFrame(0),//アニメーション１枚にかかるフレーム
	m_attackAttributes(AttackAttributes::Null),//攻撃属性
	m_startAttackFrame(0),//攻撃の発生
	m_finishAttackFrame(0),//攻撃持続終了
	m_giveNoActFrame(0),//硬直
	m_giveGuardFrame(0),//ガード硬直
	m_giveDamage(0),//ダメージ
	m_giveAttackVelo(),//攻撃によって相手に与えるVelo
	m_commandIndex(0),
	lastAttackType(AttackTypes::Null),
	m_idleStandHandel(LoadGraph("./img/playerbase/idle_001.png")),//待機
	m_idleSquatHandel(LoadGraph("./img/playerbase/idle_squat_001.png")),//しゃがみ待機
	m_walkFrontHandel(LoadGraph("./img/playerbase/walk_front_001.png")),//前歩き
	m_walkBackHandel(LoadGraph("./img/playerbase/walk_back_001.png")),//後ろ歩き
	m_jumpHandel(LoadGraph("./img/playerbase/jump_vertical_001.png")),//ジャンプ
	//立ち
	m_punchLightHandle(LoadGraph("./img/punch/punch_stand_002.png")),//弱パンチ
	m_punchHighHandle(LoadGraph("./img/punch/punch_stand_001.png")),//強パンチ
	m_kickLightHandle(LoadGraph("./img/kick/kick_stand_001.png")),//弱キック
	m_kickHighHandle(LoadGraph("./img/kick/kick_stand_002.png")),//強キック
	//しゃがみ
	m_punchLightSquatHandle(LoadGraph("./img/punch/punch_squat_001.png")),//しゃがみ弱パンチ
	m_punchHighSquatHandle(LoadGraph("./img/punch/punch_squat_003.png")),//しゃがみ強パンチ
	m_kickLightSquatHandle(LoadGraph("./img/kick/kick_squat_001.png")),//しゃがみ弱キック
	m_kickHighSquatHandle(LoadGraph("./img/kick/kick_squat_003.png")),//しゃがみ強キック
	//空中
	m_punchLightAerialHandle(LoadGraph("./img/punch/punch_aerial_001.png")),//空中弱パンチ
	m_punchHighAerialHandle(LoadGraph("./img/punch/punch_aerial_003.png")),//空中強パンチ
	m_kickLightAerialHandle(LoadGraph("./img/kick/kick_aerial_002.png")),//空中弱キック
	m_kickHighAerialHandle(LoadGraph("./img/kick/kick_aerial_001.png")),//空中強キック

	m_guardStandHandle(LoadGraph("./img/guard/guard_stand_001.png")),//立ちガード
	m_guardSquatHandle(LoadGraph("./img/guard/guard_squat_002.png")),//しゃがみガード

	//投げ
	m_graspHandle(LoadGraph("./img/throw/grasp_001.png")),//つかみ
	m_throwFrontHandle(LoadGraph("./img/throw/throw_001.png")),//前投げ
	m_throwBackHandle(LoadGraph("./img/throw/throw_002.png")),//後ろ投げ


	m_damageStandHandle(LoadGraph("./img/hit/hit_stand_001.png")),//喰らい
	m_damageAerialHandle(LoadGraph("./img/hit/hit_aerial_001.png")),//喰らい
	m_downHandle(LoadGraph("./img/playerbase/down_001.png")),//ダウン
	m_downAerialHandle(LoadGraph("./img/playerbase/down_aerial_001.png")),//空中ダウン
	m_standUpHandle(LoadGraph("./img/playerbase/standUp.png")),//起き上がり

	m_commandHandle1(LoadGraph("./img/waza/hadou_001.png")),//コマンド技
	m_commandHandle2(LoadGraph("./img/waza/shouryuu.png")),//コマンド技
	m_commandHandle3(LoadGraph("./img/waza/sukuryuu.png")),//コマンド技
	m_commandHandle4(LoadGraph("./img/waza/spairaruaro.png")),//コマンド技
	m_commandHandle5(LoadGraph("./img/waza/sonic.png")),//コマンド技
	m_commandHandle6(LoadGraph("./img/waza/wave.png")),//コマンド技
	m_commandHandle7(LoadGraph("./img/waza/rolling.png")),//コマンド技
	m_commandHandle8(LoadGraph("./img/waza/tatumaki.png")),//コマンド技
	m_commandHandle9(LoadGraph("./img/waza/wildhant.png")),//コマンド技


	m_winPoseHandle(LoadGraph("./img/playerbase/win_001.png"))//勝利ポーズ
{
	for (int i = 0; i < 3;++i)
	{
		m_selectCommandIndex[i] = index[i];
	}
}

Chara::~Chara()
{
}

bool Chara::CheckMyCommand()
{
	for (int i = 0;i < 3;++i)
	{
		//自分の選んだコマンドのインデックスと一致するならtrue
		if (m_commandIndex == m_selectCommandIndex[i])
		{
			return true;
		}
	}
	//失敗
	m_commandIndex = 0;
	return false;
}

bool Chara::SuccessCommandCheck(Input& input, Player& player)
{
	//いったん空にする
	m_commandIndex = static_cast<int>(Command::Null);
	if (player.GetAttackAttackTypes() == AttackTypes::LightPunch || 
		player.GetAttackAttackTypes() == AttackTypes::HighPunch)
	{
		if (input.CheckDirCommand("RightOneRevolution") ||
			input.CheckDirCommand("LeftOneRevolution"))
		{
			m_commandIndex = static_cast<int>(Command::Sukuryuu);
			return CheckMyCommand();
		}
		//昇竜拳
		if (input.CheckDirCommand("623") && !player.GetDirState() ||//右向き
			input.CheckDirCommand("421") && player.GetDirState() ||//左向き
			input.CheckDirCommand("323") && !player.GetDirState() ||
			input.CheckDirCommand("121") && player.GetDirState() ||
			input.CheckDirCommand("636") && !player.GetDirState() ||
			input.CheckDirCommand("414") && player.GetDirState() )
		{
			m_commandIndex = static_cast<int>(Command::Shouryuken);
			return CheckMyCommand();
		}

		if (input.CheckDirCommand("HalfTurnRightStart") && !player.GetDirState() ||
			input.CheckDirCommand("HalfTurnLeftStart") && player.GetDirState())
		{
			//ワイルドハント
			m_commandIndex = static_cast<int>(Command::Wildhant);
			return CheckMyCommand();
		}

		//波動拳
		if (input.CheckDirCommand("236") && !player.GetDirState() ||
			input.CheckDirCommand("214") && player.GetDirState())
		{
			//波動拳
			m_commandIndex = static_cast<int>(Command::Hadouken);
			return CheckMyCommand();
		}
		if (
			input.CheckKeepCommand("4K6") && !player.GetDirState() ||
			input.CheckKeepCommand("6K4") && player.GetDirState() ||
			//斜めタメ
			input.CheckKeepCommand("1K6") && !player.GetDirState() ||
			input.CheckKeepCommand("3K4") && player.GetDirState())
		{
			m_commandIndex = static_cast<int>(Command::Sonic);
			return CheckMyCommand();
		}
	}

	if (player.GetAttackAttackTypes() == AttackTypes::LightKick ||
		player.GetAttackAttackTypes() == AttackTypes::HighKick)
	{
		//竜巻
		if (input.CheckDirCommand("214"))
		{
			if (!player.GetDirState())
			{
				m_commandIndex = static_cast<int>(Command::Tatumaki);
				return CheckMyCommand();
			}
			else
			{
				m_commandIndex = static_cast<int>(Command::Wave);
				return CheckMyCommand();
			}
		}
		if (input.CheckDirCommand("236"))
		{
			if (!player.GetDirState())
			{
				m_commandIndex = static_cast<int>(Command::Wave);
				return CheckMyCommand();
			}
			else
			{
				m_commandIndex = static_cast<int>(Command::Tatumaki);
				return CheckMyCommand();
			}
		}
	
		//後ろ溜め
		if (
			input.CheckKeepCommand("4K6") && !player.GetDirState() ||
			input.CheckKeepCommand("6K4") && player.GetDirState() ||
			//斜めタメ
			input.CheckKeepCommand("1K6") && !player.GetDirState() ||
			input.CheckKeepCommand("3K4") && player.GetDirState())
		{
			m_commandIndex = static_cast<int>(Command::Supairaru);
			return CheckMyCommand();
		}
		if (
			//下タメ上要素(サマーソルト)
			input.CheckKeepCommand("2K8") ||
			input.CheckKeepCommand("2K7") ||
			input.CheckKeepCommand("2K9") ||
			//斜め下タメ上要素
			input.CheckKeepCommand("1K8") ||
			input.CheckKeepCommand("1K7") ||
			input.CheckKeepCommand("1K9") ||
			input.CheckKeepCommand("3K8") ||
			input.CheckKeepCommand("3K7") ||
			input.CheckKeepCommand("3K9"))
		{
			m_commandIndex = static_cast<int>(Command::Rolling);
			return CheckMyCommand();
		}
	}

	return false;
}


//コマンド技モーション取得
void Chara::GetAnimCommand(Player& player)
{
	if (m_commandIndex == static_cast<int>(Command::Hadouken))
	{
		GetAnimCommand1(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Shouryuken))
	{
		GetAnimCommand2(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Sukuryuu))
	{
		GetAnimCommand3(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Supairaru))
	{
		GetAnimCommand4(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Sonic))
	{
		GetAnimCommand5(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Wave))
	{
		GetAnimCommand6(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Rolling))
	{
		GetAnimCommand7(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Tatumaki))
	{
		GetAnimCommand8(player);
	}
}
//コマンド技モーションの当たり判定(右向き)
void Chara::GetHitBoxCommand(Player& player)
{
	if (m_commandIndex == static_cast<int>(Command::Hadouken))
	{
		GetHitBoxCommand1(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Shouryuken))
	{
		GetHitBoxCommand2(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Sukuryuu))
	{
		GetHitBoxCommand3(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Supairaru))
	{
		GetHitBoxCommand4(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Sonic))
	{
		GetHitBoxCommand5(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Wave))
	{
		GetHitBoxCommand6(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Rolling))
	{
		GetHitBoxCommand7(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Tatumaki))
	{
		GetHitBoxCommand8(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Wildhant))
	{
		GetHitBoxCommand9(player);
	}
}
//コマンド技が相手にもたらす効果
void Chara::GetGiveEffectCommand(Player& player)
{
	if (m_commandIndex == static_cast<int>(Command::Hadouken))
	{
		GetGiveEffectCommand1(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Shouryuken))
	{
		GetGiveEffectCommand2(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Sukuryuu))
	{
		GetGiveEffectCommand3(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Supairaru))
	{
		GetGiveEffectCommand4(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Sonic))
	{
		GetGiveEffectCommand5(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Wave))
	{
		GetGiveEffectCommand6(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Rolling))
	{
		GetGiveEffectCommand7(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Tatumaki))
	{
		GetGiveEffectCommand8(player);
	}
	else if (m_commandIndex == static_cast<int>(Command::Wildhant))
	{
		GetGiveEffectCommand9(player);
	}
}
//コマンド技の内容（動き）
void Chara::MovementCommand(Player& player, Bullet& bullet, Player& enemy)
{
	//移動量リセット
	player.SetVeloX(0);
	if (m_commandIndex == static_cast<int>(Command::Hadouken))
	{
		MovementCommand1(player, bullet, enemy);
	}
	else if (m_commandIndex == static_cast<int>(Command::Shouryuken))
	{
		MovementCommand2(player, bullet, enemy);
	}
	else if (m_commandIndex == static_cast<int>(Command::Sukuryuu))
	{
		MovementCommand3(player, bullet, enemy);
	}
	else if (m_commandIndex == static_cast<int>(Command::Supairaru))
	{
		MovementCommand4(player, bullet, enemy);
	}
	else if (m_commandIndex == static_cast<int>(Command::Sonic))
	{
		MovementCommand5(player, bullet, enemy);
	}
	else if (m_commandIndex == static_cast<int>(Command::Wave))
	{
		MovementCommand6(player, bullet, enemy);
	}
	else if (m_commandIndex == static_cast<int>(Command::Rolling))
	{
		MovementCommand7(player, bullet, enemy);
	}
	else if (m_commandIndex == static_cast<int>(Command::Tatumaki))
	{
		MovementCommand8(player, bullet, enemy);
	}
	else if (m_commandIndex == static_cast<int>(Command::Wildhant))
	{
		MovementCommand9(player, bullet, enemy);
	}
}

//立ち待機モーション
void Chara::GetAnimIdleStand(Player& player)
{
	/// <summary>
	/// 画像のデータ
	/// </summary>
	m_handle = m_idleStandHandel;//待機
	/// <summary>
	/// アニメーションの枚数
	/// </summary>
	m_animNum = 6;
	/// <summary>
	/// １つのアニメーションにかかるフレーム
	/// </summary>
	m_oneAnimFrame = 4;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
//待機モーションの当たり判定(右向き)
void Chara::GetHitBoxIdleStand(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}

;
//しゃがみ待機モーション
void Chara::GetAnimIdleSquat(Player& player)
{
	m_handle = m_idleSquatHandel;//しゃがみ待機
	m_animNum = 6;
	m_oneAnimFrame = 4;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
void Chara::GetHitBoxIdleSquat(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = 0;
	headBox.y1 = -40;
	headBox.x2 = 90;
	headBox.y2 = 50;
	//体	
	bodyBox.x1 = -40;
	bodyBox.y1 = 50;
	bodyBox.x2 = 60;
	bodyBox.y2 = 120;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 120;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
;
//前歩きモーション
void Chara::GetAnimWalkFront(Player& player)
{
	m_handle = m_walkFrontHandel;//歩き
	m_animNum = 8;
	m_oneAnimFrame = 4;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
void Chara::GetHitBoxWalkFront(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
;
//後ろ歩きモーション
void Chara::GetAnimWalkBack(Player& player)
{
	m_handle = m_walkBackHandel;//歩き
	m_animNum = 8;
	m_oneAnimFrame = 4;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
void Chara::GetHitBoxWalkBack(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
;
//ジャンプモーション
void Chara::GetAnimJump(Player& player)
{
	m_handle = m_jumpHandel;//ジャンプ
	m_animNum = 10;
	m_oneAnimFrame = 4;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
void Chara::GetHitBoxJump(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 130;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
;
//弱パンチモーション
void Chara::GetAnimPunchLight(Player& player)
{
	m_handle = m_punchLightHandle;//弱パンチ
	//全体フレーム14
	m_animNum = 7;
	m_oneAnimFrame = 2;
	m_startAttackFrame = kStartFramePunchLight;//攻撃発生
	m_finishAttackFrame = kFinishFramePunchLight;//持続終了

	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
//弱パンチモーションの当たり判定(右向き)
void Chara::GetHitBoxPunchLight(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 60;
	attackBox.y1 = -50;
	attackBox.x2 = 200;
	attackBox.y2 = 20;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectPunchLight(Player& player)
{
	m_giveDamage = 3.0f;
	m_giveNoActFrame = kAllFramePunchLight - kStartFramePunchLight + 4;
	m_giveGuardFrame = kAllFramePunchLight - kStartFramePunchLight - 1;
	m_attackAttributes = AttackAttributes::Upper;//上段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 3;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}
;
//強パンチモーション
void Chara::GetAnimPunchHigh(Player& player)
{
	m_handle = m_punchHighHandle;//強パンチ
	//全体フレーム22
	m_animNum = 11;
	m_oneAnimFrame = 2;
	m_startAttackFrame = kStartFramePunchHigh;//攻撃発生
	m_finishAttackFrame = 16;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxPunchHigh(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 90;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 80;
	attackBox.y1 = -40;
	attackBox.x2 = 220;
	attackBox.y2 = 60;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectPunchHigh(Player& player)
{
	m_giveDamage = 8.0f;
	m_giveNoActFrame = kAllFramePunchHigh - kStartFramePunchHigh + 4;
	m_giveGuardFrame = kAllFramePunchHigh - kStartFramePunchHigh - 2;
	m_attackAttributes = AttackAttributes::Upper;//上段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 10;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
};


//弱キックモーション
void Chara::GetAnimKickLight(Player& player)
{
	//全体フレーム18
	m_handle = m_kickLightHandle;//弱キック
	m_animNum = 9;
	m_oneAnimFrame = 2;
	m_startAttackFrame = kStartFrameKickLight;//攻撃発生
	m_finishAttackFrame = 7;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxKickLight(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 80;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 40;
	attackBox.y1 = 20;
	attackBox.x2 = 210;
	attackBox.y2 = 80;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectKickLight(Player& player)
{
	m_giveDamage = 3.0f;
	m_giveNoActFrame = kAllFrameKickLight - kStartFrameKickLight + 2;
	m_giveGuardFrame = kAllFrameKickLight - kStartFrameKickLight - 4;
	m_attackAttributes = AttackAttributes::Upper;//上段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 7;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}



;
//強キックモーション
void Chara::GetAnimKickHigh(Player& player)
{
	//全体フレーム33
	m_handle = m_kickHighHandle;//強キック
	m_animNum = 11;
	m_oneAnimFrame = 3;
	m_startAttackFrame = kStartFrameKickHigh;//攻撃発生
	m_finishAttackFrame = 15;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxKickHigh(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 130;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 150;
	attackBox.y1 = -120;
	attackBox.x2 = 280;
	attackBox.y2 = -40;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectKickHigh(Player& player)
{
	m_giveDamage = 9.0f;
	m_giveNoActFrame = kAllFrameKickHigh - kStartFrameKickHigh + 9;
	m_giveGuardFrame = kAllFrameKickHigh - kStartFrameKickHigh + 1;
	m_attackAttributes = AttackAttributes::Upper;//上段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 7;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}

//しゃがみ弱パンチ
void Chara::GetAnimPunchLightSquat(Player& player)
{
	//全体フレーム8
	m_handle = m_punchLightSquatHandle;//しゃがみ弱パンチ
	m_animNum = 8;
	m_oneAnimFrame = 1;
	m_startAttackFrame = kStartFramePunchLightSquat;//攻撃発生
	m_finishAttackFrame = 5;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxPunchLightSquat(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = 0;
	headBox.y1 = -40;
	headBox.x2 = 90;
	headBox.y2 = 50;
	//体	
	bodyBox.x1 = -40;
	bodyBox.y1 = 50;
	bodyBox.x2 = 60;
	bodyBox.y2 = 120;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 120;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 100;
	attackBox.y1 = 40;
	attackBox.x2 = 200;
	attackBox.y2 = 120;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectPunchLightSquat(Player& player)
{
	m_giveDamage = 2.0f;
	m_giveNoActFrame = kAllFramePunchLightSquat - kStartFramePunchLightSquat + 4;
	m_giveGuardFrame = kAllFramePunchLightSquat - kStartFramePunchLightSquat - 1;
	m_attackAttributes = AttackAttributes::Upper;//上段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 10;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}

//しゃがみ強パンチ
void Chara::GetAnimPunchHighSquat(Player& player)
{
	//全体フレーム26
	m_handle = m_punchHighSquatHandle;//しゃがみ弱パンチ
	m_animNum = 13;
	m_oneAnimFrame = 2;
	m_startAttackFrame = kStartFramePunchHighSquat;//攻撃発生
	m_finishAttackFrame = 14;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxPunchHighSquat(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = 0;
	headBox.y1 = -40;
	headBox.x2 = 90;
	headBox.y2 = 50;
	//体	
	bodyBox.x1 = -40;
	bodyBox.y1 = 50;
	bodyBox.x2 = 60;
	bodyBox.y2 = 120;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 120;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 20;
	attackBox.y1 = -200;
	attackBox.x2 = 120;
	attackBox.y2 = -100;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectPunchHighSquat(Player& player)
{
	m_giveDamage = 8.0f;
	m_giveNoActFrame = kAllFramePunchHighSquat - kStartFramePunchHighSquat + 1;
	m_giveGuardFrame = kAllFramePunchHighSquat - kStartFramePunchHighSquat - 7;
	m_attackAttributes = AttackAttributes::Upper;//上段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 10;
	m_giveAttackVelo.y = 10;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}

//しゃがみ弱キック
void Chara::GetAnimKickLightSquat(Player& player)
{
	//全体フレーム14
	m_handle = m_kickLightSquatHandle;//しゃがみ弱パンチ
	m_animNum = 7;
	m_oneAnimFrame = 2;
	m_startAttackFrame = kStartFrameKickLightSquat;//攻撃発生
	m_finishAttackFrame = 6;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxKickLightSquat(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = 0;
	headBox.y1 = -40;
	headBox.x2 = 90;
	headBox.y2 = 50;
	//体	
	bodyBox.x1 = -40;
	bodyBox.y1 = 50;
	bodyBox.x2 = 60;
	bodyBox.y2 = 120;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 120;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 60;
	attackBox.y1 = 120;
	attackBox.x2 = 150;
	attackBox.y2 = 180;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectKickLightSquat(Player& player)
{
	m_giveDamage = 2.0f;
	m_giveNoActFrame = kAllFrameKickLightSquat - kStartFrameKickLightSquat + 3;
	m_giveGuardFrame = kAllFrameKickLightSquat - kStartFrameKickLightSquat - 3;
	m_attackAttributes = AttackAttributes::Lower;//下段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 5;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}


//しゃがみ強キック
void Chara::GetAnimKickHighSquat(Player& player)
{
	//全体フレーム32
	m_handle = m_kickHighSquatHandle;//しゃがみ強キック
	m_animNum = 8;
	m_oneAnimFrame = 4;
	m_startAttackFrame = kStartFrameKickHighSquat;//攻撃発生
	m_finishAttackFrame = 11;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxKickHighSquat(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = 0;
	headBox.y1 = -40;
	headBox.x2 = 90;
	headBox.y2 = 50;
	//体	
	bodyBox.x1 = -40;
	bodyBox.y1 = 50;
	bodyBox.x2 = 60;
	bodyBox.y2 = 120;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 120;
	legBox.x2 = 140;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 140;
	attackBox.y1 = 120;
	attackBox.x2 = 250;
	attackBox.y2 = 200;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectKickHighSquat(Player& player)
{
	m_giveDamage = 2.0f;
	m_giveNoActFrame = kDown;
	m_giveGuardFrame = kAllFrameKickHighSquat - kStartFrameKickHighSquat - 12;
	m_attackAttributes = AttackAttributes::Lower;//下段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 2;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}

//空中弱パンチ
void Chara::GetAnimPunchLightAerial(Player& player)
{
	m_handle = m_punchLightAerialHandle;//空中弱パンチ
	//全体フレーム16
	m_animNum = 8;
	m_oneAnimFrame = 1;
	m_startAttackFrame = 4;//攻撃発生
	m_finishAttackFrame = 13;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxPunchLightAerial(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -100;
	headBox.x2 = 200;
	headBox.y2 = -10;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 130;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = 100;
	attackBox.y1 = 0;
	attackBox.x2 = 200;
	attackBox.y2 = 150;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectPunchLightAerial(Player& player)
{
	m_giveDamage = 3.0f;
	m_giveNoActFrame = 13;
	m_giveGuardFrame = 9;
	m_attackAttributes = AttackAttributes::middle;//中段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 3;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}
;

//空中強パンチ
void Chara::GetAnimPunchHighAerial(Player& player)
{
	m_handle = m_punchHighAerialHandle;//空中弱パンチ
	//全体フレーム16
	m_animNum = 10;
	m_oneAnimFrame = 2;
	m_startAttackFrame = 9;//攻撃発生
	m_finishAttackFrame = 14;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxPunchHighAerial(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -50;
	headBox.x2 = 200;
	headBox.y2 = 100;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 130;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = 100;
	attackBox.y1 = 50;
	attackBox.x2 = 200;
	attackBox.y2 = 220;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetGiveEffectPunchHighAerial(Player& player)
{
	m_giveDamage = 8.0f;
	m_giveNoActFrame = 19;
	m_giveGuardFrame = 15;
	m_attackAttributes = AttackAttributes::middle;//中段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 5;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}

//空中弱キック
void Chara::GetAnimKickLightAerial(Player& player)
{
	m_handle = m_kickLightAerialHandle;//空中弱キック
	//全体フレーム16
	m_animNum = 10;
	m_oneAnimFrame = 2;
	m_startAttackFrame = 5;//攻撃発生
	m_finishAttackFrame = 6;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}

void Chara::GetHitBoxKickLightAerial(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 80;
	legBox.y2 = 130;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = 100;
	attackBox.y1 = 100;
	attackBox.x2 = 220;
	attackBox.y2 = 200;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}

void Chara::GetGiveEffectKickLightAerial(Player& player)
{
	m_giveDamage = 2.0f;
	m_giveNoActFrame = 14;
	m_giveGuardFrame = 10;
	m_attackAttributes = AttackAttributes::middle;//中段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 3;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}


//空中強キック
void Chara::GetAnimKickHighAerial(Player& player)
{
	m_handle = m_kickHighAerialHandle;//空中弱キック
	//全体フレーム16
	m_animNum = 11;
	m_oneAnimFrame = 2;
	m_startAttackFrame = 12;//攻撃発生
	m_finishAttackFrame = 19;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}

void Chara::GetHitBoxKickHighAerial(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 170;
	legBox.y2 = 170;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 100;
	attackBox.x2 = 220;
	attackBox.y2 = 220;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);

}

void Chara::GetGiveEffectKickHighAerial(Player& player)
{
	m_giveDamage = 8.0f;
	m_giveNoActFrame = 19;
	m_giveGuardFrame = 15;
	m_attackAttributes = AttackAttributes::middle;//中段
	player.SetGiveDamage(m_giveDamage);
	player.SetGiveNoActFrame(m_giveNoActFrame);
	player.SetGiveGuardFrame(m_giveGuardFrame);
	player.SetAttackAttributes(m_attackAttributes);
	//相手を動かす
	m_giveAttackVelo.x = 5;
	m_giveAttackVelo.y = 0;
	player.SetGiveAttackVelo(m_giveAttackVelo);
}




//ガードモーション
void Chara::GetAnimGuardStand(Player& player)
{
	m_handle = m_guardStandHandle;//ガード
	m_animNum = 6;
	m_oneAnimFrame =1;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
void Chara::GetAnimGuardSquat(Player& player)
{
	m_handle = m_guardSquatHandle;//ガード
	m_animNum = 8;
	m_oneAnimFrame = 1;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
//つかみモーション
void Chara::GetAnimGrasp(Player& player)
{
	//全体フレーム30
	m_handle = m_graspHandle;//ガード
	m_animNum = 6;
	m_oneAnimFrame = 5;
	m_startAttackFrame = 5;//攻撃発生
	m_finishAttackFrame = 7;//持続終了
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxGrasp(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box graspBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 80;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 90;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 70;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//投げ判定
	graspBox.x1 = 10;
	graspBox.y1 = -100;
	graspBox.x2 = 110;
	graspBox.y2 = 150;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxGrasp(graspBox);
}
void Chara::GetGiveEffectGrasp(Player& player)
{
	//ダメージ
	m_giveDamage = 12.0f;
	player.SetGiveDamage(m_giveDamage);
}
//投げられ当たり判定
void Chara::GetHitBoxBeThrown(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	
	//頭	
	headBox.x1 = 0;
	headBox.y1 = 0;
	headBox.x2 = 0;
	headBox.y2 = 0;
	//体	
	bodyBox.x1 = 0;
	bodyBox.y1 = 0;
	bodyBox.x2 = 0;
	bodyBox.y2 = 0;
	//足	
	legBox.x1 = 0;
	legBox.y1 = 0;
	legBox.x2 = 0;
	legBox.y2 = 0;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	
}

//前投げ
void Chara::GetAnimThrowFront(Player& player)
{
	m_handle = m_throwFrontHandle;
	m_animNum = 12;
	m_oneAnimFrame = 6;player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
void Chara::GetHitBoxThrowFront(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	//頭	50x50
	headBox.x1 = 0;
	headBox.y1 = 0;
	headBox.x2 = 0;
	headBox.y2 = 0;
	//体	70x70
	bodyBox.x1 = 0;
	bodyBox.y1 = 0;
	bodyBox.x2 = 0;
	bodyBox.y2 = 0;
	//足	70x70
	legBox.x1 = 0;
	legBox.y1 = 0;
	legBox.x2 = 0;
	legBox.y2 = 0;
	//投げやられ 100x110
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	
}

//後ろ投げ
void Chara::GetAnimThrowBack(Player& player)
{
	m_handle = m_throwBackHandle;
	m_animNum = 9;
	m_oneAnimFrame = 6;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
void Chara::GetHitBoxThrowBack(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	//頭	50x50
	headBox.x1 = 0;
	headBox.y1 = 0;
	headBox.x2 = 0;
	headBox.y2 = 0;
	//体	70x70
	bodyBox.x1 = 0;
	bodyBox.y1 = 0;
	bodyBox.x2 = 0;
	bodyBox.y2 = 0;
	//足	70x70
	legBox.x1 = 0;
	legBox.y1 = 0;
	legBox.x2 = 0;
	legBox.y2 = 0;
	//投げやられ 100x110
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
}
;
//喰らい(やられ)モーション
//立ちやられ
void Chara::GetAnimHitStand(Player& player)
{
	m_handle = m_damageStandHandle;//やられ
	m_animNum = 5;
	m_oneAnimFrame = 5;
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
//空中やられ
void Chara::GetAnimHitAerial(Player& player)
{
	m_handle = m_damageAerialHandle;//やられ
	m_animNum = 7;
	m_oneAnimFrame = 4;
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
//やられモーションの当たり判定(右向き)
void Chara::GetHitBoxHitStand(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
//ダウンモーション
void Chara::GetAnimDown(Player& player)
{
	m_handle = m_downHandle;
	m_animNum = 13 ;
	m_oneAnimFrame = 3;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
void Chara::GetHitBoxDown(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	50x50
	headBox.x1 = 0;
	headBox.y1 = 0;
	headBox.x2 = 0;
	headBox.y2 = 0;
	//体	70x70
	bodyBox.x1 = 0;
	bodyBox.y1 = 0;
	bodyBox.x2 = 0;
	bodyBox.y2 = 0;
	//足	70x70
	legBox.x1 = 0;
	legBox.y1 = 0;
	legBox.x2 = 0;
	legBox.y2 = 0;
	//投げやられ 100x110
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
void Chara::GetAnimDownAerial(Player& player)
{
	m_handle = m_downAerialHandle;
	m_animNum = 8;
	m_oneAnimFrame = 3;
	player.SetHandle(m_handle);//昇竜拳
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
void Chara::GetHitBoxDownAerial(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	//頭	50x50
	headBox.x1 = 0;
	headBox.y1 = 0;
	headBox.x2 = 0;
	headBox.y2 = 0;
	//体	70x70
	bodyBox.x1 = 0;
	bodyBox.y1 = 0;
	bodyBox.x2 = 0;
	bodyBox.y2 = 0;
	//足	70x70
	legBox.x1 = 0;
	legBox.y1 = 0;
	legBox.x2 = 0;
	legBox.y2 = 0;
	//投げやられ 100x110
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	
}
//起き上がり
void Chara::GetAnimStandUp(Player& player)
{
	m_handle = m_standUpHandle;
	m_animNum = 5;
	m_oneAnimFrame = 3;
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}
;
//勝利ポーズ
void Chara::GetAnimWinPose(Player& player)
{
	m_handle = m_winPoseHandle;
	m_animNum = 5;
	m_oneAnimFrame = 5;
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
}



//波動拳モーション
void Chara::GetAnimCommand1(Player& player)
{
	m_handle = m_commandHandle1;//波動拳
	//全体フレーム47
	m_animNum = 7;
	m_oneAnimFrame = 3;
	//攻撃発生
	if (player.GetAttackAttackTypes() == AttackTypes::HighPunch)
	{
		m_oneAnimFrame = 2;
		//強は発生12フレーム
		m_startAttackFrame = kStartFrameCommand1High;
	}
	else if (player.GetAttackAttackTypes() == AttackTypes::LightPunch)
	{
		//弱は発生16フレーム
		m_startAttackFrame = kStartFrameCommand1Light;
	}
	m_finishAttackFrame = 47;//持続終了
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxCommand1(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	Box graspBox;
	//頭	
	headBox.x1 = -20;
	headBox.y1 = -150;
	headBox.x2 = 100;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 120;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 120;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//投げ
	graspBox.x1 = 0;
	graspBox.y1 = 0;
	graspBox.x2 = 0;
	graspBox.y2 = 0;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	player.SetHitBoxGrasp(graspBox);
}

void Chara::GetGiveEffectCommand1(Player& player)
{
	return;
}

void Chara::MovementCommand1(Player& player,Bullet& bullet, Player& enemy)
{

	if (player.GetAttackAttackTypes() == AttackTypes::HighPunch)
	{
		//強は発生12フレーム
		if (player.GetAnimCountFrame() == kStartFrameCommand1High)
		{
			//弾（波動拳を出す）
			bullet.SetShotMove(player.GetPos(), player.GetDirState(), kHadouSpeedHigh);
			bullet.SetShotEffect(6.0f,		//ダメージ
				kAllFrameCommand1 - kStartFrameCommand1High - 3,//ヒット時の硬直
				kAllFrameCommand1 - kStartFrameCommand1High - 9);//ガードしている敵に当たった時の硬直
			bullet.LoadBullet1Handle();
			bullet.OnIsShooting();
		}
	}
	else if (player.GetAttackAttackTypes() == AttackTypes::LightPunch)
	{
		//弱は発生16フレーム
		if (player.GetAnimCountFrame() == kStartFrameCommand1Light)
		{
			//弾（波動拳を出す）
			bullet.SetShotMove(player.GetPos(), player.GetDirState(), kHadouSpeedLight);
			bullet.SetShotEffect(6.0f,
				kAllFrameCommand1 - kStartFrameCommand1Light + 1,
				kAllFrameCommand1 - kStartFrameCommand1Light - 5);
			bullet.LoadBullet1Handle();
			bullet.OnIsShooting();
		}
	}

}
;
//昇竜拳モーション
void Chara::GetAnimCommand2(Player& player)
{
	//強版
	if (player.GetAttackAttackTypes() == AttackTypes::HighPunch)
	{
		//全体フレーム60
		m_animNum = 4;
		m_oneAnimFrame = 3;
		m_startAttackFrame = kStartFrameCommand2 + 2;
		m_finishAttackFrame = 14 + 2;
	}
	//弱版
	else if (player.GetAttackAttackTypes() == AttackTypes::LightPunch)
	{
		//全体フレーム47
		m_animNum = 4;
		m_oneAnimFrame = 2;
		m_startAttackFrame = kStartFrameCommand2;//攻撃発生
		m_finishAttackFrame = 14;//持続終了
	}
	m_handle = m_commandHandle2;//昇竜拳
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}
void Chara::GetHitBoxCommand2(Player& player)
{	
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	Box graspBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = 60;
	attackBox.y1 = -200;
	attackBox.x2 = 160;
	attackBox.y2 = 50;
	//投げ
	graspBox.x1 = 0;
	graspBox.y1 = 0;
	graspBox.x2 = 0;
	graspBox.y2 = 0;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	player.SetHitBoxGrasp(graspBox);
}
void Chara::GetGiveEffectCommand2(Player& player)
{
	//強版
	if (player.GetAttackAttackTypes() == AttackTypes::HighPunch)
	{
		//全体フレーム60
		//無敵
		player.ResetHitBox();
		m_velocity.y = -46.0f;
		player.SetGiveDamage(12.0f);
		player.SetGiveNoActFrame(kDown);
		player.SetGiveGuardFrame(kAllFrameCommand2 - kStartFrameCommand2 - 39);
		player.SetAttackAttributes(AttackAttributes::Upper);//上段
		m_giveAttackVelo.x = 5;
		m_giveAttackVelo.y = -40;
		player.SetGiveAttackVelo(m_giveAttackVelo);
	}
	//弱版
	else if (player.GetAttackAttackTypes() == AttackTypes::LightPunch)
	{
		//全体フレーム47
		m_velocity.y = -34.0f;
		player.SetGiveDamage(8.0f);
		player.SetGiveNoActFrame(kDown);
		player.SetGiveGuardFrame(kAllFrameCommand2 - kStartFrameCommand2 - 23);
		player.SetAttackAttributes(AttackAttributes::Upper);//上段
		m_giveAttackVelo.x = 5;
		m_giveAttackVelo.y = -30;
		player.SetGiveAttackVelo(m_giveAttackVelo);
	}
}
void Chara::MovementCommand2(Player& player, Bullet& bullet, Player& enemy)
{
	//だんだん上昇していく
	m_velocity.y -= 0.2f;
	//昇竜拳の動き
	m_velocity.x = 10;

	if (player.GetDirState())
	{
		m_velocity.x *= -1;	
	}
	
	//攻撃持続終了
	if (player.GetAnimCountFrame() > m_finishAttackFrame)
	{
		m_velocity.x = 0;
		m_velocity.y += 0.5f;
	}
	//攻撃判定が出てから登り始める
	if (player.GetAnimCountFrame() > (m_startAttackFrame + 2))
	{
		player.SetVelo(m_velocity);
	}
}

void Chara::GetAnimCommand3(Player& player)
{
	if (player.GetIsThrowSuccess())
	{
		m_handle = m_commandHandle3;//スクリュー
		//全体フレーム120
		m_animNum = 8;
		m_oneAnimFrame = 15;
		player.SetHandle(m_handle);
		player.SetAnimNum(m_animNum);
		player.SetOneAnimFrame(m_oneAnimFrame);
	}
	else
	{
		//投げのアニメーション
		//投げのつかみモーションが30フレーム(6枚*5ずつ)なので
		//2倍にして60フレームにしとく
		GetAnimGrasp(player);
		player.SetOneAnimFrame(10);
	}
}

void Chara::GetHitBoxCommand3(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	Box graspBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 80;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 90;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 70;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃判定
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//弱版
	//投げ
	graspBox.x1 = 0;
	graspBox.y1 = 0;
	graspBox.x2 = 170;
	graspBox.y2 = 100;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	player.SetHitBoxGrasp(graspBox);
}

void Chara::GetGiveEffectCommand3(Player& player)
{
	m_velocity.x = 0.0f;
	m_velocity.y = -30.0f;
	//強版
	if (player.GetAttackAttackTypes() == AttackTypes::HighPunch)
	{
		//強版
		//投げ
		Box graspBox;
		graspBox.x1 = 0;
		graspBox.y1 = 0;
		graspBox.x2 = 120;
		graspBox.y2 = 100;

		player.SetHitBoxGrasp(graspBox);
		player.SetGiveDamage(33.0f);
	}
	//弱版
	else if (player.GetAttackAttackTypes() == AttackTypes::LightPunch)
	{
		player.SetGiveDamage(25.0f);
	}
}

void Chara::MovementCommand3(Player& player, Bullet& bullet, Player& enemy)
{
	GetAnimCommand3(player);
	//投げが当たったら
	if (player.GetIsThrowSuccess())
	{
		//投げモーション
		GetAnimCommand3(player);

		//上昇
		if (player.GetAnimCountFrame() < 30)
		{
			player.SetVelo(m_velocity);
			enemy.SetPos(player.GetPos());
		}
		else
		{
			//落下中もプレイヤーと同じ座標
			if (!player.GetIsGround())
			{
				enemy.SetPos(player.GetPos());
			}
			else	//地面についたら
			{

				//投げる
				//相手をふっとばす方向
				Vector3 throwVelo;
				throwVelo.x = 20;

				if (player.GetDirState())//自分が左向きなら左に飛ばす
				{
					throwVelo.x *= -1;
				}

				throwVelo.y = -30;

				//セット
				enemy.SetThrowVelo(throwVelo);
				// 投げられ状態にする
				enemy.OnIsThrown();

				//投げの後相手との距離を離す
				//自分が左向きなら右に動く
				//自分が右向きなら左に動く
				m_velocity.y = 0;
				m_velocity.x = 5.0f;
				if (!player.GetDirState())
				{
					m_velocity.x *= -1;
				}
				player.SetVelo(m_velocity);
				//リセット
				m_velocity.x = 0;
			}
		}
	}
}

//スパイラルアロー
void Chara::GetAnimCommand4(Player& player)
{
	//強版
	if (player.GetAttackAttackTypes() == AttackTypes::HighKick)
	{
		//全体フレーム49
		m_animNum = 7;
		m_oneAnimFrame = 7;
		m_startAttackFrame = kStartFrameCommand4High;//攻撃発生
		m_finishAttackFrame = 30;//持続終了
	}
	//弱版
	else if (player.GetAttackAttackTypes() == AttackTypes::LightKick)
	{
		//全体フレーム42
		m_animNum = 7;
		m_oneAnimFrame = 6;
		m_startAttackFrame = kStartFrameCommand4Light;//攻撃発生
		m_finishAttackFrame = 21;//持続終了
	}
	m_handle = m_commandHandle4;//スパイラルアロー
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}

void Chara::GetHitBoxCommand4(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	Box graspBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 80;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 90;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 70;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃判定
	attackBox.x1 = 0;
	attackBox.y1 = -20;
	attackBox.x2 = 250;
	attackBox.y2 = 100;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
}

void Chara::GetGiveEffectCommand4(Player& player)
{	
	//移動量
	m_velocity.y = 0.0f;
	//強版
	if (player.GetAttackAttackTypes() == AttackTypes::HighKick)
	{
		m_velocity.x = 20.0f;
		if (player.GetDirState())
		{
			m_velocity.x *= -1;
		}
		player.SetGiveDamage(8.0f);
		player.SetGiveNoActFrame(kDown);
		player.SetGiveGuardFrame(kAllFrameCommand4High - kStartFrameCommand4High - 12);
		player.SetAttackAttributes(AttackAttributes::Upper);//上段
		m_giveAttackVelo.x = 10.0f;
		m_giveAttackVelo.y = -10.0f;
		player.SetGiveAttackVelo(m_giveAttackVelo);
	}
	//弱版
	else if (player.GetAttackAttackTypes() == AttackTypes::LightKick)
	{
		m_velocity.x = 15.0f;
		if (player.GetDirState())
		{
			m_velocity.x *= -1;
		}
		player.SetGiveDamage(8.0f);
		player.SetGiveNoActFrame(kDown);
		player.SetGiveGuardFrame(kAllFrameCommand4Light - kStartFrameCommand4Light - 12);
		player.SetAttackAttributes(AttackAttributes::Upper);//上段
		m_giveAttackVelo.x = 5.0f;
		m_giveAttackVelo.y = -10.0f;
		player.SetGiveAttackVelo(m_giveAttackVelo);
	}

}

void Chara::MovementCommand4(Player& player, Bullet& bullet, Player& enemy)
{
	//攻撃判定が出る少し前から動き始める
	if (player.GetAnimCountFrame() > (m_startAttackFrame * 0.5f))
	{
		player.SetVelo(m_velocity);
	}
}

void Chara::GetAnimCommand5(Player& player)
{
	m_handle = m_commandHandle5;//ソニックブーム
	//全体フレーム40
	m_animNum = 7;
	m_oneAnimFrame = 6;
	//攻撃発生
	if (player.GetAttackAttackTypes() == AttackTypes::HighPunch)
	{
		m_oneAnimFrame = 5;
		//強は発生12フレーム
		m_startAttackFrame = 10;
	}
	else if (player.GetAttackAttackTypes() == AttackTypes::LightPunch)
	{
		//弱は発生16フレーム
		m_startAttackFrame = 10;
	}
	m_finishAttackFrame = 40;//持続終了
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}

void Chara::GetHitBoxCommand5(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	Box graspBox;
	//頭	
	headBox.x1 = -20;
	headBox.y1 = -150;
	headBox.x2 = 100;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 120;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 120;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//投げ
	graspBox.x1 = 0;
	graspBox.y1 = 0;
	graspBox.x2 = 0;
	graspBox.y2 = 0;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	player.SetHitBoxGrasp(graspBox);
}

void Chara::GetGiveEffectCommand5(Player& player)
{
	return;
}

void Chara::MovementCommand5(Player& player, Bullet& bullet, Player& enemy)
{
	if (player.GetAttackAttackTypes() == AttackTypes::HighPunch)
	{
		//強は発生12フレーム
		if (player.GetAnimCountFrame() == kStartFrameCommand1High)
		{
			//弾（波動拳を出す）
			bullet.SetShotMove(player.GetPos(), player.GetDirState(), kSonicSpeedHigh);
			bullet.SetShotEffect(5.5f,		//ダメージ
				kAllFrameCommand1 - kStartFrameCommand5 + 3,//ヒット時の硬直
				kAllFrameCommand1 - kStartFrameCommand5 - 3);//ガードしている敵に当たった時の硬直
			bullet.LoadBullet2Handle();
			bullet.OnIsShooting();
		}
	}
	else if (player.GetAttackAttackTypes() == AttackTypes::LightPunch)
	{
		//弱は発生16フレーム
		if (player.GetAnimCountFrame() == kStartFrameCommand1Light)
		{
			//弾（波動拳を出す）
			bullet.SetShotMove(player.GetPos(), player.GetDirState(), kSonicSpeedLight);
			bullet.SetShotEffect(5.5f,
				kAllFrameCommand1 - kStartFrameCommand5 + 3,
				kAllFrameCommand1 - kStartFrameCommand5 - 3);
			bullet.LoadBullet2Handle();
			bullet.OnIsShooting();
		}
	}
}

void Chara::GetAnimCommand6(Player& player)
{
	m_handle = m_commandHandle6;
	//全体フレーム45
	m_animNum = 10;
	m_oneAnimFrame = 4;
	//攻撃発生
	if (player.GetAttackAttackTypes() == AttackTypes::HighKick)
	{
		m_oneAnimFrame = 3;
		//強は発生16フレーム
		m_startAttackFrame = kStartFrameCommand6High;
		m_finishAttackFrame = kAllFrameCommand6High;//持続終了 52F
	}
	else if (player.GetAttackAttackTypes() == AttackTypes::LightKick)
	{
		//弱は発生16フレーム
		m_startAttackFrame = kStartFrameCommand6Light;
		m_finishAttackFrame = kAllFrameCommand6Light;//持続終了 46F
	}
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}

void Chara::GetHitBoxCommand6(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	Box graspBox;
	//頭	
	headBox.x1 = -20;
	headBox.y1 = -150;
	headBox.x2 = 100;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 120;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 120;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;
	//投げ
	graspBox.x1 = 0;
	graspBox.y1 = 0;
	graspBox.x2 = 0;
	graspBox.y2 = 0;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	player.SetHitBoxGrasp(graspBox);
}

void Chara::GetGiveEffectCommand6(Player& player)
{
	return;
}

void Chara::MovementCommand6(Player& player, Bullet& bullet, Player& enemy)
{
	if (player.GetAttackAttackTypes() == AttackTypes::HighKick)
	{
		//強は発生12フレーム
		if (player.GetAnimCountFrame() == kStartFrameCommand1High)
		{
			//弾（波動拳を出す）
			bullet.SetShotMove(player.GetPos(), player.GetDirState(), kWaveSpeedHigh);
			bullet.SetShotEffect(4.0f,		//ダメージ
				kAllFrameCommand1 - kStartFrameCommand6High + 3,//ヒット時の硬直
				kAllFrameCommand1 - kStartFrameCommand6High - 3);//ガードしている敵に当たった時の硬直
			bullet.LoadBullet3Handle();
			bullet.OnIsShooting();
		}
	}
	else if (player.GetAttackAttackTypes() == AttackTypes::LightKick)
	{
		//弱は発生16フレーム
		if (player.GetAnimCountFrame() == kStartFrameCommand1Light)
		{
			//弾（波動拳を出す）
			bullet.SetShotMove(player.GetPos(), player.GetDirState(), kWaveSpeedLight);
			bullet.SetShotEffect(4.0f,
				kAllFrameCommand1 - kStartFrameCommand6Light + 3,
				kAllFrameCommand1 - kStartFrameCommand6Light - 3);
			bullet.LoadBullet3Handle();
			bullet.OnIsShooting();
		}
	}
}

void Chara::GetAnimCommand7(Player& player)
{
	//全体フレーム48
	m_animNum = 6;
	m_oneAnimFrame = 4;
	m_startAttackFrame = kStartFrameCommand7;//攻撃発生
	m_finishAttackFrame = 27;//持続終了
	m_handle = m_commandHandle7;//バーチカルローリング
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}

void Chara::GetHitBoxCommand7(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	Box graspBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = -50;
	attackBox.y1 = -120;
	attackBox.x2 = 150;
	attackBox.y2 = 100;
	//投げ
	graspBox.x1 = 0;
	graspBox.y1 = 0;
	graspBox.x2 = 0;
	graspBox.y2 = 0;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	player.SetHitBoxGrasp(graspBox);
}

void Chara::GetGiveEffectCommand7(Player& player)
{
	//強版
	if (player.GetAttackAttackTypes() == AttackTypes::HighKick)
	{
		//全体フレーム48
		//無敵
		player.ResetHitBox();
		m_velocity.y = -10.0f;
		m_velocity.x = 0.0f;
		player.SetGiveDamage(12.0f);
		player.SetGiveNoActFrame(kDown);
		player.SetGiveGuardFrame(kAllFrameCommand7 - kStartFrameCommand7 - 27);
		player.SetAttackAttributes(AttackAttributes::Upper);//上段
		m_giveAttackVelo.x = 5;
		m_giveAttackVelo.y = -40;
		player.SetGiveAttackVelo(m_giveAttackVelo);
	}
	//弱版
	else if (player.GetAttackAttackTypes() == AttackTypes::LightKick)
	{
		//全体フレーム48
		m_velocity.y = -10.0f;
		m_velocity.x = 20.0f;
		if (player.GetDirState())
		{
			m_velocity.x *= -1;
		}
		player.SetGiveDamage(8.0f);
		player.SetGiveNoActFrame(kDown);
		player.SetGiveGuardFrame(kAllFrameCommand7 - kStartFrameCommand7 - 27);
		player.SetAttackAttributes(AttackAttributes::Upper);//上段
		m_giveAttackVelo.x = 5;
		m_giveAttackVelo.y = -30;
		player.SetGiveAttackVelo(m_giveAttackVelo);
	}
}

void Chara::MovementCommand7(Player& player, Bullet& bullet, Player& enemy)
{
	////攻撃持続終了まで上り続ける
	if (player.GetAnimCountFrame() > m_finishAttackFrame)
	{
		m_velocity.x = 0;
	}
	//攻撃判定が出てから登り始める
	if (player.GetAnimCountFrame() > (m_startAttackFrame * 0.5f))
	{
		player.SetVelo(m_velocity);
	}
}

void Chara::GetAnimCommand8(Player& player)
{
	m_animNum = 7;
	m_oneAnimFrame = 5;
	m_startAttackFrame = 4;//攻撃発生
	m_finishAttackFrame = 5;//持続終了
	m_handle = m_commandHandle8;//竜巻旋風脚
	player.SetHandle(m_handle);
	player.SetAnimNum(m_animNum);
	player.SetOneAnimFrame(m_oneAnimFrame);
	player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
	player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
}

void Chara::GetHitBoxCommand8(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	Box graspBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 60;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 70;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 60;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = 0;
	throwBox.y1 = 0;
	throwBox.x2 = 0;
	throwBox.y2 = 0;
	//攻撃
	attackBox.x1 = -150;
	attackBox.y1 = 0;
	attackBox.x2 = 150;
	attackBox.y2 = 100;
	//投げ
	graspBox.x1 = 0;
	graspBox.y1 = 0;
	graspBox.x2 = 0;
	graspBox.y2 = 0;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	player.SetHitBoxGrasp(graspBox);
}

void Chara::GetGiveEffectCommand8(Player& player)
{
	m_velocity.y = 0.0f;
	//強版
	if (player.GetAttackAttackTypes() == AttackTypes::HighKick)
	{
		//全体フレーム61
		m_allFrame = kAllFrameCommand8High;
		m_velocity.x = 10.0f;
		//強版は持続長い
		m_startAttackFrame = 14;//攻撃発生
		m_finishAttackFrame = 15;//持続終了
		player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
		player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了

		player.SetGiveDamage(5.0f);
		player.SetGiveNoActFrame(m_allFrame - 27);
		player.SetGiveGuardFrame(m_allFrame - 12);
		player.SetAttackAttributes(AttackAttributes::Upper);//上段
		m_giveAttackVelo.x = 10;
		m_giveAttackVelo.y = 0;
		player.SetGiveAttackVelo(m_giveAttackVelo);
	}
	//弱版
	else if (player.GetAttackAttackTypes() == AttackTypes::LightKick)
	{
		//全体フレーム48
		m_allFrame = kAllFrameCommand8Light;
		m_velocity.x = 5.0f;
		player.SetGiveDamage(2.5f);
		player.SetGiveNoActFrame(m_allFrame - 27);
		player.SetGiveGuardFrame(m_allFrame - 14);
		player.SetAttackAttributes(AttackAttributes::Upper);//上段
		m_giveAttackVelo.x = 10;
		m_giveAttackVelo.y = 0;
		player.SetGiveAttackVelo(m_giveAttackVelo);
	}
	if (player.GetDirState())
	{
		m_velocity.x *= -1;
	}
}

void Chara::MovementCommand8(Player& player, Bullet& bullet, Player& enemy)
{
	player.SetVelo(m_velocity);
	if (player.GetAnimCountFrame() < m_allFrame)
	{
		//持続終了タイミング
		if (player.GetAnimCountFrame() == m_finishAttackFrame)
		{
			if (player.GetAnimCountFrame() < (m_allFrame * 0.5f))
			{
				m_startAttackFrame += 10;
				m_finishAttackFrame += 10;
				player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
				player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
				//攻撃
				Box attackBox;
				attackBox.x1 = -150;
				attackBox.y1 = 0;
				attackBox.x2 = 150;
				attackBox.y2 = 100;
				player.SetHitBoxAttack(attackBox);
				player.SetGiveNoActFrame(kDown);
				player.SetGiveGuardFrame(m_allFrame - m_startAttackFrame - 14);
			}
		}
		if (player.GetAnimIndex() >= (m_animNum - 1))
		{
			player.SetAnimIndex(0);
		}
	}
	else
	{
		player.SetAnimIndex(m_animNum);
	}
}

void Chara::GetAnimCommand9(Player& player)
{
	if (player.GetIsThrowSuccess())
	{
		m_handle = m_commandHandle3;//スクリュー
		//全体フレーム40
		m_animNum = 8;
		m_oneAnimFrame = 5;
		player.SetHandle(m_handle);
		player.SetAnimNum(m_animNum);
		player.SetOneAnimFrame(m_oneAnimFrame);
	}
	else
	{
		m_handle = m_commandHandle9;//とびかかる
		m_animNum = 8;
		
		if (player.GetAttackAttackTypes() == AttackTypes::HighPunch)
		{
			m_oneAnimFrame = 7;
			m_startAttackFrame = 43;//攻撃発生
			m_finishAttackFrame = 45;//持続終了
		}
		else if (player.GetAttackAttackTypes() == AttackTypes::LightPunch)
		{	
			m_oneAnimFrame = 6;
			m_startAttackFrame = 34;//攻撃発生
			m_finishAttackFrame = 36;//持続終了
		}
		player.SetHandle(m_handle);//昇竜拳
		player.SetAnimNum(m_animNum);
		player.SetOneAnimFrame(m_oneAnimFrame);
		player.SetStartAttackFrame(m_startAttackFrame);//攻撃発生
		player.SetFinishAttackFrame(m_finishAttackFrame);//持続終了
	}
}

void Chara::GetHitBoxCommand9(Player& player)
{
	Box headBox;
	Box bodyBox;
	Box legBox;
	Box throwBox;
	Box attackBox;
	Box graspBox;
	//頭	
	headBox.x1 = -10;
	headBox.y1 = -150;
	headBox.x2 = 80;
	headBox.y2 = -60;
	//体	
	bodyBox.x1 = -60;
	bodyBox.y1 = -60;
	bodyBox.x2 = 90;
	bodyBox.y2 = 60;
	//足	
	legBox.x1 = -70;
	legBox.y1 = 60;
	legBox.x2 = 70;
	legBox.y2 = 180;
	//投げやられ 
	throwBox.x1 = -100;
	throwBox.y1 = 0;
	throwBox.x2 = 100;
	throwBox.y2 = 200;
	//攻撃判定
	attackBox.x1 = 0;
	attackBox.y1 = 0;
	attackBox.x2 = 0;
	attackBox.y2 = 0;

	//投げ
	graspBox.x1 = 0;
	graspBox.y1 = 0;
	graspBox.x2 = 130;
	graspBox.y2 = 100;

	//当たり判定セット
	player.SetHitBoxHead(headBox);
	player.SetHitBoxBody(bodyBox);
	player.SetHitBoxLeg(legBox);
	player.SetHitBoxThrow(throwBox);
	player.SetHitBoxAttack(attackBox);
	player.SetHitBoxGrasp(graspBox);
}

void Chara::GetGiveEffectCommand9(Player& player)
{
	//強版
	if (player.GetAttackAttackTypes() == AttackTypes::HighPunch)
	{
		m_velocity.x = 15.0f;
		m_velocity.y = -55.0f;
		player.SetGiveDamage(18.0f);
	}
	//弱版
	else if (player.GetAttackAttackTypes() == AttackTypes::LightPunch)
	{
		m_velocity.x = 5.0f;
		m_velocity.y = -50.0f;
		player.SetGiveDamage(16.0f);
	}
	if (player.GetDirState())
	{
		m_velocity.x *= -1;
	}
	player.SetVelo(m_velocity);
}

void Chara::MovementCommand9(Player& player, Bullet& bullet, Player& enemy)
{
	GetAnimCommand9(player);
	if (player.GetAnimCountFrame() > 0)
	{
		if (player.GetIsGround())
		{
			m_velocity.x = 0;
			m_velocity.y = 0;
		}
	}
	//投げが当たったら
	if (player.GetIsThrowSuccess())
	{
		m_velocity.x = 0;
		m_velocity.y = 0;
		if (player.GetAnimIndex() > 3)
		{
			//投げる
			//相手をふっとばす方向
			Vector3 throwVelo;
			throwVelo.x = 20;

			if (player.GetDirState())//自分が左向きなら左に飛ばす
			{
				throwVelo.x *= -1;
			}

			throwVelo.y = -30;

			//セット
			enemy.SetThrowVelo(throwVelo);
			// 投げられ状態にする
			enemy.OnIsThrown();

			//投げの後相手との距離を離す
			//自分が左向きなら右に動く
			//自分が右向きなら左に動く
			m_velocity.y = 0;
			m_velocity.x = 5.0f;
			if (!player.GetDirState())
			{
				m_velocity.x *= -1;
			}
			player.SetVelo(m_velocity);
			//リセット
			m_velocity.x = 0;
		}
		else
		{
			enemy.SetPos(player.GetPos());
		}
	}
	player.SetVeloX(m_velocity.x);
}

