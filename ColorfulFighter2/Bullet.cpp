#include "Bullet.h"
#include <DxLib.h>
#include "Camera.h"
#include "Player.h"
#include "Camera.h"

namespace
{
	constexpr float kBulletOffsetPos = 90.0f;

	//弾の大きさ
	constexpr int kBulletScale = 5.0f;
	//弾のアニメーションの速さ
	constexpr int kOneAnimFrame = 6;

	//波動拳
	//弾の判定
	constexpr int kHadouRadius = 16.0f * kBulletScale;
	//弾の番号
	constexpr int kStartHadouAnimIndex = 211;
	constexpr int kFinishHadouAnimIndex = 214;
	//弾の位置の調整
	constexpr int kHadouOffsetPosX = 100;
	
	//ソニックブーム
	//弾の判定
	constexpr int kSonicRadius = 16.0f * kBulletScale;
	//弾の番号
	constexpr int kStartSonicAnimIndex = 231;
	constexpr int kFinishSonicAnimIndex = 234;

	//パワーウェイブ
	//弾の判定
	constexpr int kPowerWaveRadius = 16.0f * kBulletScale;
	//弾の番号
	constexpr int kStartPowerWaveAnimIndex = 6;
	constexpr int kFinishPowerWaveAnimIndex = 9;
	//弾の位置の調整
	constexpr int kPowerWaveOffsetPosY = 120;
}

Bullet::Bullet(PlayerIndex playerIndex):
	m_pos(),
	m_velocity(),
	m_playerIndex(playerIndex),
	m_isShooting(false),
	m_hitBoxAttack(-kHadouRadius,-kHadouRadius, kHadouRadius, kHadouRadius),
	m_isHitPlayer(false),
	m_giveNoActFrame(0),
	m_giveGuardFrame(0),
	m_giveDamage(5.0f),
	m_giveAttackVelo(5,0,0),
	m_animIndex(kStartHadouAnimIndex),
	m_startAnimIndex(kStartHadouAnimIndex),
	m_finishAnimIndex(kFinishHadouAnimIndex),
	m_animCountFrame(0)
{
	m_blueBulletHandle = LoadGraph("./img/Bullet/BlueBullet32x32.png");//青
	m_yellowBulletHandle = LoadGraph("./img/Bullet/YellowBullet32x32.png");//黄色
	m_purpleBulletHandle = LoadGraph("./img/Bullet/PurpleBullet32x32.png");//紫
	m_bulletHandle = m_blueBulletHandle;
}

Bullet::~Bullet()
{

}

void Bullet::Init()
{
	m_pos.x = 0;
	m_pos.y = 0;
	m_velocity.x = 0;
	m_velocity.y = 0;
	m_isShooting = false;
	InitHitBoxAttack();
	m_isHitPlayer = false;
}

void Bullet::SetShotMove(Vector3 pos, bool isLeft, float speed)
{
	//すでに弾が発射されているならやらない
	if (m_isShooting)return;
	//ポジションとスピードと向きをセット
	m_pos.y = pos.y;
	if (isLeft)
	{
		m_pos.x = pos.x - kBulletOffsetPos;
		m_velocity.x = -speed;
	}
	else
	{
		m_pos.x = pos.x + kBulletOffsetPos;
		m_velocity.x = speed;
	}
	InitHitBoxAttack();
}

void Bullet::SetShotEffect(float damage, int giveNoActFrame, int giveGuardFrame)
{
	//すでに弾が発射されているならやらない
	if (m_isShooting)return;
	//相手に与える影響をセット
	m_giveDamage = damage;
	m_giveNoActFrame = giveNoActFrame;
	m_giveGuardFrame = giveGuardFrame;
}

void Bullet::Update(Player& enemy, Bullet& otherBullet, Camera& camera)
{
	if (m_isShooting)
	{
		//移動
		m_pos += m_velocity;

		//敵に当たったかどうかをチェック
		//当たった後の処理はCollisionCheckがやる（1P側と2P側で処理の順で優劣がつかないようにするため）
		m_isHitPlayer = HitCheckPlayer(enemy);

		//弾同士で相殺
		if (HitCheckBullet(otherBullet) && otherBullet.GetIsShooting())
		{
			Disappear();
			otherBullet.Disappear();
		}
		//壁の外にいったら消す
		if (camera.GetCameraLeftWallPos() - m_hitBoxAttack.x2 > m_pos.x || camera.GetCameraRightWallPos() - m_hitBoxAttack.x1 < m_pos.x)
		{
			Disappear();
		}
		
		m_animCountFrame++;
		//アニメーションのフレームを数える
		if (m_animCountFrame % kOneAnimFrame == 0 && m_animCountFrame != 0)
		{
			m_animIndex++;
			//アニメーションの数が最大まで行ったとき
			if ((m_animIndex > m_finishAnimIndex))
			{
				m_animIndex = m_startAnimIndex;
			}
		}
	}
	
}

void Bullet::Draw(Camera& camera)
{
	////弾
	if (m_isShooting)
	{
		//左に進んでるなら左を向く
		bool isLeft = false;
		if (m_velocity.x < 0)
		{
			isLeft = true;
		}

		//切り取るを計算する
		int sizeX, sizeY;
		GetGraphSize(m_bulletHandle, &sizeX, &sizeY);//画像サイズ
		int cutX = m_animIndex % (sizeX / 32);//横
		int cutY = m_animIndex / (sizeX / 32);//縦
		//描画
		////メイン
		DrawRectRotaGraphFast(static_cast<int>(m_pos.x) + static_cast<int>(camera.m_drawOffset.x),
			static_cast<int>(m_pos.y) + static_cast<int>(camera.m_drawOffset.y),
			32 * cutX,
			32 * cutY,
			32, 32,
			kBulletScale, 0.0f, m_bulletHandle, true, isLeft);


#if _DEBUG
		//当たり判定
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		//攻撃判定
		DxLib::DrawBox(
			(static_cast<int>(m_pos.x) + m_hitBoxAttack.x1) + camera.m_drawOffset.x,
			(static_cast<int>(m_pos.y) + m_hitBoxAttack.y1) + camera.m_drawOffset.y,
			(static_cast<int>(m_pos.x) + m_hitBoxAttack.x2) + camera.m_drawOffset.x,
			(static_cast<int>(m_pos.y) + m_hitBoxAttack.y2) + camera.m_drawOffset.y,
			0xff0000, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
#endif
	}
	if (m_isHitPlayer)
	{
		DrawString(0, 600, "波動拳ヒット", 0xff0055);
	}
}

void Bullet::Disappear()
{
	m_isShooting = false;
	ResetAttackBox();
	ResetIsHitPlayer();
}

//波動拳
void Bullet::LoadBullet1Handle()
{
	//すでに弾が発射されているならやらない
	if (m_isShooting)return;
	//波動拳
	m_bulletHandle = m_blueBulletHandle;
	m_startAnimIndex = kStartHadouAnimIndex;
	m_finishAnimIndex = kFinishHadouAnimIndex;
	m_animIndex = m_startAnimIndex;
	m_animCountFrame = 0;
	//手から出したいので位置調整
	if (m_velocity.x < 0)
	{
		m_pos.x -= kHadouOffsetPosX;
	}
	else
	{
		m_pos.x += kHadouOffsetPosX;
	}
	
}
//ソニックブーム
void Bullet::LoadBullet2Handle()
{
	//すでに弾が発射されているならやらない
	if (m_isShooting)return;
	//ソニックブーム
	m_bulletHandle = m_yellowBulletHandle;
	m_startAnimIndex = kStartSonicAnimIndex;
	m_finishAnimIndex = kFinishSonicAnimIndex;
	m_animIndex = m_startAnimIndex;
	m_animCountFrame = 0;
}
//闇払い
void Bullet::LoadBullet3Handle()
{
	//すでに弾が発射されているならやらない
	if (m_isShooting)return;
	//闇払い
	m_bulletHandle = m_purpleBulletHandle;
	m_startAnimIndex = kStartPowerWaveAnimIndex;
	m_finishAnimIndex = kFinishPowerWaveAnimIndex;
	m_animIndex = m_startAnimIndex;
	m_animCountFrame = 0;
	//地面を滑らせたいので位置調整
	m_pos.y += kPowerWaveOffsetPosY;
}

bool Bullet::HitCheckPlayer(Player& enemy)
{
	//攻撃の判定がないならチェックしない
	if (m_hitBoxAttack.x1 == 0 &&
		m_hitBoxAttack.y1 == 0 &&
		m_hitBoxAttack.x2 == 0 &&
		m_hitBoxAttack.y2 == 0)	return false;



	float width;
	float height;
	bool isHit = false;
	if (enemy.GetDirState())
	{
		//攻撃判定の中心を求める
		Vector3 attackPos((m_pos.x + m_hitBoxAttack.x2 + m_pos.x + m_hitBoxAttack.x1) / 2,
			(m_pos.y + m_hitBoxAttack.y2 + m_pos.y + m_hitBoxAttack.y1) / 2, 0.0f);

		//当たり判定があるならチェック
		//頭
		if (enemy.GetHitBoxHead().x1 != 0 ||
			enemy.GetHitBoxHead().y1 != 0 ||
			enemy.GetHitBoxHead().x2 != 0 ||
			enemy.GetHitBoxHead().y2 != 0)
		{
			//敵の頭のやられ判定////////////////////////////////////////
			Vector3 enemyHead((enemy.GetPos().x - enemy.GetHitBoxHead().x2 + enemy.GetPos().x - enemy.GetHitBoxHead().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxHead().y2 + enemy.GetPos().y + enemy.GetHitBoxHead().y1) / 2, 0.0f);
			width = std::abs(attackPos.x - enemyHead.x);
			height = std::abs(attackPos.y - enemyHead.y);

			//X方向の攻撃の幅と体の幅を足した値より小さいなら当たってる
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxHead().x2 - enemy.GetHitBoxHead().x1) / 2) &&
				//Y方向攻撃の幅と体の幅を足した値より小さいなら当たってる
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxHead().y2 - enemy.GetHitBoxHead().y1) / 2)) return true;
			/////////////////////////////////////////////////////////////
		}
	

		//当たり判定があるならチェック
		//体
		if (enemy.GetHitBoxBody().x1 != 0 ||
			enemy.GetHitBoxBody().y1 != 0 ||
			enemy.GetHitBoxBody().x2 != 0 ||
			enemy.GetHitBoxBody().y2 != 0)
		{
			//敵の体のやられ判定////////////////////////////////////////////
			Vector3 enemyBody((enemy.GetPos().x - enemy.GetHitBoxBody().x2 + enemy.GetPos().x - enemy.GetHitBoxBody().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxBody().y2 + enemy.GetPos().y + enemy.GetHitBoxBody().y1) / 2, 0.0f);
			width = std::abs(attackPos.x - enemyBody.x);
			height = std::abs(attackPos.y - enemyBody.y);

			//X方向の攻撃の幅と体の幅を足した値より小さいなら当たってる
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxBody().x2 - enemy.GetHitBoxBody().x1) / 2) &&
				//Y方向攻撃の幅と体の幅を足した値より小さいなら当たってる
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxBody().y2 - enemy.GetHitBoxBody().y1) / 2)) return true;
			///////////////////////////////////////////////////////////////
		}
		
		//当たり判定があるならチェック
		//足
		if (enemy.GetHitBoxLeg().x1 != 0 ||
			enemy.GetHitBoxLeg().y1 != 0 ||
			enemy.GetHitBoxLeg().x2 != 0 ||
			enemy.GetHitBoxLeg().y2 != 0)
		{
			//敵の足のやられ判定
			Vector3 enemyLeg((enemy.GetPos().x - enemy.GetHitBoxLeg().x2 + enemy.GetPos().x - enemy.GetHitBoxLeg().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxLeg().y2 + enemy.GetPos().y + enemy.GetHitBoxLeg().y1) / 2, 0.0f);
			width = std::abs(attackPos.x - enemyLeg.x);
			height = std::abs(attackPos.y - enemyLeg.y);

			//X方向の攻撃の幅と体の幅を足した値より小さいなら当たってる
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxLeg().x2 - enemy.GetHitBoxLeg().x1) / 2) &&
				//Y方向攻撃の幅と体の幅を足した値より小さいなら当たってる
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxLeg().y2 - enemy.GetHitBoxLeg().y1) / 2)) return true;
		}
		
	}
	else
	{
		//攻撃判定の中心を求める
		Vector3 attackPos((m_pos.x - m_hitBoxAttack.x2 + m_pos.x - m_hitBoxAttack.x1) / 2,
			(m_pos.y + m_hitBoxAttack.y2 + m_pos.y + m_hitBoxAttack.y1) / 2, 0.0f);

		//頭
		if (enemy.GetHitBoxHead().x1 != 0 ||
			enemy.GetHitBoxHead().y1 != 0 ||
			enemy.GetHitBoxHead().x2 != 0 ||
			enemy.GetHitBoxHead().y2 != 0)
		{
			//敵の頭のやられ判定////////////////////////////////////////
			Vector3 enemyHead((enemy.GetPos().x + enemy.GetHitBoxHead().x2 + enemy.GetPos().x + enemy.GetHitBoxHead().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxHead().y2 + enemy.GetPos().y + enemy.GetHitBoxHead().y1) / 2, 0.0f);

			width = std::abs(attackPos.x - enemyHead.x);
			height = std::abs(attackPos.y - enemyHead.y);

			//X方向の攻撃の幅と頭の幅を足した値より小さいなら当たってる
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxHead().x2 - enemy.GetHitBoxHead().x1) / 2) &&
				//X方向の攻撃の幅と頭の幅を足した値より小さいなら当たってる
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxHead().y2 - enemy.GetHitBoxHead().y1) / 2)) return true;
			////////////////////////////////////////////////////////////
		}
	

		//体
		if (enemy.GetHitBoxBody().x1 != 0 ||
			enemy.GetHitBoxBody().y1 != 0 ||
			enemy.GetHitBoxBody().x2 != 0 ||
			enemy.GetHitBoxBody().y2 != 0)
		{
			//敵の体のやられ判定///////////////////////////////////////
			Vector3 enemyBody((enemy.GetPos().x + enemy.GetHitBoxBody().x2 + enemy.GetPos().x + enemy.GetHitBoxBody().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxBody().y2 + enemy.GetPos().y + enemy.GetHitBoxBody().y1) / 2, 0.0f);

			width = std::abs(attackPos.x - enemyBody.x);
			height = std::abs(attackPos.y - enemyBody.y);

			//X方向の攻撃の幅と体の幅を足した値より小さいなら当たってる
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxBody().x2 - enemy.GetHitBoxBody().x1) / 2) &&
				//Y方向攻撃の幅と体の幅を足した値より小さいなら当たってる
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxBody().y2 - enemy.GetHitBoxBody().y1) / 2)) return true;
			/////////////////////////////////////////////////////////
		}
	


		//足
		if (enemy.GetHitBoxLeg().x1 != 0 ||
			enemy.GetHitBoxLeg().y1 != 0 ||
			enemy.GetHitBoxLeg().x2 != 0 ||
			enemy.GetHitBoxLeg().y2 != 0)
		{
			//敵の足のやられ判定
			Vector3 enemyLeg((enemy.GetPos().x + enemy.GetHitBoxLeg().x2 + enemy.GetPos().x + enemy.GetHitBoxLeg().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxLeg().y2 + enemy.GetPos().y + enemy.GetHitBoxLeg().y1) / 2, 0.0f);
			width = std::abs(attackPos.x - enemyLeg.x);
			height = std::abs(attackPos.y - enemyLeg.y);

			//X方向の攻撃の幅と体の幅を足した値より小さいなら当たってる
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxLeg().x2 - enemy.GetHitBoxLeg().x1) / 2) &&
				//Y方向攻撃の幅と体の幅を足した値より小さいなら当たってる
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxLeg().y2 - enemy.GetHitBoxLeg().y1) / 2)) return true;
		}
	
	}



	return false;
}

bool Bullet::HitCheckBullet(Bullet& otherBullet)
{

	//X、Y方向の離れている距離を絶対値化して取得
	float width = std::abs(m_pos.x - otherBullet.GetPos().x);
	float height = std::abs(m_pos.y - otherBullet.GetPos().y);

	//X方向の距離がキャラの幅より大きければ当たっていない
	if (width > std::abs(GetHitBoxAttack().x1 - otherBullet.GetHitBoxAttack().x2))	return false;
	//Y方向の距離がキャラの高さより大きければ当たっていない
	if (height > std::abs(GetHitBoxAttack().y1 - otherBullet.GetHitBoxAttack().y2))	return false;

	//ここまでくれば当たってる
	return true;
}

void Bullet::InitHitBoxAttack()
{
	//攻撃範囲
	m_hitBoxAttack.x1 = -kHadouRadius;
	m_hitBoxAttack.y1 = -kHadouRadius;
	m_hitBoxAttack.x2 = kHadouRadius;
	m_hitBoxAttack.y2 = kHadouRadius;
}

