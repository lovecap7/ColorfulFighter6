#include "Bullet.h"
#include <DxLib.h>
#include "Camera.h"
#include "Player.h"
#include "Camera.h"

namespace
{
	constexpr float kBulletOffsetPos = 90.0f;

	//�e�̑傫��
	constexpr int kBulletScale = 5.0f;
	//�e�̃A�j���[�V�����̑���
	constexpr int kOneAnimFrame = 6;

	//�g����
	//�e�̔���
	constexpr int kHadouRadius = 16.0f * kBulletScale;
	//�e�̔ԍ�
	constexpr int kStartHadouAnimIndex = 211;
	constexpr int kFinishHadouAnimIndex = 214;
	//�e�̈ʒu�̒���
	constexpr int kHadouOffsetPosX = 100;
	
	//�\�j�b�N�u�[��
	//�e�̔���
	constexpr int kSonicRadius = 16.0f * kBulletScale;
	//�e�̔ԍ�
	constexpr int kStartSonicAnimIndex = 231;
	constexpr int kFinishSonicAnimIndex = 234;

	//�p���[�E�F�C�u
	//�e�̔���
	constexpr int kPowerWaveRadius = 16.0f * kBulletScale;
	//�e�̔ԍ�
	constexpr int kStartPowerWaveAnimIndex = 6;
	constexpr int kFinishPowerWaveAnimIndex = 9;
	//�e�̈ʒu�̒���
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
	m_blueBulletHandle = LoadGraph("./img/Bullet/BlueBullet32x32.png");//��
	m_yellowBulletHandle = LoadGraph("./img/Bullet/YellowBullet32x32.png");//���F
	m_purpleBulletHandle = LoadGraph("./img/Bullet/PurpleBullet32x32.png");//��
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
	//���łɒe�����˂���Ă���Ȃ���Ȃ�
	if (m_isShooting)return;
	//�|�W�V�����ƃX�s�[�h�ƌ������Z�b�g
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
	//���łɒe�����˂���Ă���Ȃ���Ȃ�
	if (m_isShooting)return;
	//����ɗ^����e�����Z�b�g
	m_giveDamage = damage;
	m_giveNoActFrame = giveNoActFrame;
	m_giveGuardFrame = giveGuardFrame;
}

void Bullet::Update(Player& enemy, Bullet& otherBullet, Camera& camera)
{
	if (m_isShooting)
	{
		//�ړ�
		m_pos += m_velocity;

		//�G�ɓ����������ǂ������`�F�b�N
		//����������̏�����CollisionCheck�����i1P����2P���ŏ����̏��ŗD�򂪂��Ȃ��悤�ɂ��邽�߁j
		m_isHitPlayer = HitCheckPlayer(enemy);

		//�e���m�ő��E
		if (HitCheckBullet(otherBullet) && otherBullet.GetIsShooting())
		{
			Disappear();
			otherBullet.Disappear();
		}
		//�ǂ̊O�ɂ����������
		if (camera.GetCameraLeftWallPos() - m_hitBoxAttack.x2 > m_pos.x || camera.GetCameraRightWallPos() - m_hitBoxAttack.x1 < m_pos.x)
		{
			Disappear();
		}
		
		m_animCountFrame++;
		//�A�j���[�V�����̃t���[���𐔂���
		if (m_animCountFrame % kOneAnimFrame == 0 && m_animCountFrame != 0)
		{
			m_animIndex++;
			//�A�j���[�V�����̐����ő�܂ōs�����Ƃ�
			if ((m_animIndex > m_finishAnimIndex))
			{
				m_animIndex = m_startAnimIndex;
			}
		}
	}
	
}

void Bullet::Draw(Camera& camera)
{
	////�e
	if (m_isShooting)
	{
		//���ɐi��ł�Ȃ獶������
		bool isLeft = false;
		if (m_velocity.x < 0)
		{
			isLeft = true;
		}

		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_bulletHandle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = m_animIndex % (sizeX / 32);//��
		int cutY = m_animIndex / (sizeX / 32);//�c
		//�`��
		////���C��
		DrawRectRotaGraphFast(static_cast<int>(m_pos.x) + static_cast<int>(camera.m_drawOffset.x),
			static_cast<int>(m_pos.y) + static_cast<int>(camera.m_drawOffset.y),
			32 * cutX,
			32 * cutY,
			32, 32,
			kBulletScale, 0.0f, m_bulletHandle, true, isLeft);


#if _DEBUG
		//�����蔻��
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		//�U������
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
		DrawString(0, 600, "�g�����q�b�g", 0xff0055);
	}
}

void Bullet::Disappear()
{
	m_isShooting = false;
	ResetAttackBox();
	ResetIsHitPlayer();
}

//�g����
void Bullet::LoadBullet1Handle()
{
	//���łɒe�����˂���Ă���Ȃ���Ȃ�
	if (m_isShooting)return;
	//�g����
	m_bulletHandle = m_blueBulletHandle;
	m_startAnimIndex = kStartHadouAnimIndex;
	m_finishAnimIndex = kFinishHadouAnimIndex;
	m_animIndex = m_startAnimIndex;
	m_animCountFrame = 0;
	//�肩��o�������̂ňʒu����
	if (m_velocity.x < 0)
	{
		m_pos.x -= kHadouOffsetPosX;
	}
	else
	{
		m_pos.x += kHadouOffsetPosX;
	}
	
}
//�\�j�b�N�u�[��
void Bullet::LoadBullet2Handle()
{
	//���łɒe�����˂���Ă���Ȃ���Ȃ�
	if (m_isShooting)return;
	//�\�j�b�N�u�[��
	m_bulletHandle = m_yellowBulletHandle;
	m_startAnimIndex = kStartSonicAnimIndex;
	m_finishAnimIndex = kFinishSonicAnimIndex;
	m_animIndex = m_startAnimIndex;
	m_animCountFrame = 0;
}
//�ŕ���
void Bullet::LoadBullet3Handle()
{
	//���łɒe�����˂���Ă���Ȃ���Ȃ�
	if (m_isShooting)return;
	//�ŕ���
	m_bulletHandle = m_purpleBulletHandle;
	m_startAnimIndex = kStartPowerWaveAnimIndex;
	m_finishAnimIndex = kFinishPowerWaveAnimIndex;
	m_animIndex = m_startAnimIndex;
	m_animCountFrame = 0;
	//�n�ʂ����点�����̂ňʒu����
	m_pos.y += kPowerWaveOffsetPosY;
}

bool Bullet::HitCheckPlayer(Player& enemy)
{
	//�U���̔��肪�Ȃ��Ȃ�`�F�b�N���Ȃ�
	if (m_hitBoxAttack.x1 == 0 &&
		m_hitBoxAttack.y1 == 0 &&
		m_hitBoxAttack.x2 == 0 &&
		m_hitBoxAttack.y2 == 0)	return false;



	float width;
	float height;
	bool isHit = false;
	if (enemy.GetDirState())
	{
		//�U������̒��S�����߂�
		Vector3 attackPos((m_pos.x + m_hitBoxAttack.x2 + m_pos.x + m_hitBoxAttack.x1) / 2,
			(m_pos.y + m_hitBoxAttack.y2 + m_pos.y + m_hitBoxAttack.y1) / 2, 0.0f);

		//�����蔻�肪����Ȃ�`�F�b�N
		//��
		if (enemy.GetHitBoxHead().x1 != 0 ||
			enemy.GetHitBoxHead().y1 != 0 ||
			enemy.GetHitBoxHead().x2 != 0 ||
			enemy.GetHitBoxHead().y2 != 0)
		{
			//�G�̓��̂��ꔻ��////////////////////////////////////////
			Vector3 enemyHead((enemy.GetPos().x - enemy.GetHitBoxHead().x2 + enemy.GetPos().x - enemy.GetHitBoxHead().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxHead().y2 + enemy.GetPos().y + enemy.GetHitBoxHead().y1) / 2, 0.0f);
			width = std::abs(attackPos.x - enemyHead.x);
			height = std::abs(attackPos.y - enemyHead.y);

			//X�����̍U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxHead().x2 - enemy.GetHitBoxHead().x1) / 2) &&
				//Y�����U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxHead().y2 - enemy.GetHitBoxHead().y1) / 2)) return true;
			/////////////////////////////////////////////////////////////
		}
	

		//�����蔻�肪����Ȃ�`�F�b�N
		//��
		if (enemy.GetHitBoxBody().x1 != 0 ||
			enemy.GetHitBoxBody().y1 != 0 ||
			enemy.GetHitBoxBody().x2 != 0 ||
			enemy.GetHitBoxBody().y2 != 0)
		{
			//�G�̑̂̂��ꔻ��////////////////////////////////////////////
			Vector3 enemyBody((enemy.GetPos().x - enemy.GetHitBoxBody().x2 + enemy.GetPos().x - enemy.GetHitBoxBody().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxBody().y2 + enemy.GetPos().y + enemy.GetHitBoxBody().y1) / 2, 0.0f);
			width = std::abs(attackPos.x - enemyBody.x);
			height = std::abs(attackPos.y - enemyBody.y);

			//X�����̍U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxBody().x2 - enemy.GetHitBoxBody().x1) / 2) &&
				//Y�����U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxBody().y2 - enemy.GetHitBoxBody().y1) / 2)) return true;
			///////////////////////////////////////////////////////////////
		}
		
		//�����蔻�肪����Ȃ�`�F�b�N
		//��
		if (enemy.GetHitBoxLeg().x1 != 0 ||
			enemy.GetHitBoxLeg().y1 != 0 ||
			enemy.GetHitBoxLeg().x2 != 0 ||
			enemy.GetHitBoxLeg().y2 != 0)
		{
			//�G�̑��̂��ꔻ��
			Vector3 enemyLeg((enemy.GetPos().x - enemy.GetHitBoxLeg().x2 + enemy.GetPos().x - enemy.GetHitBoxLeg().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxLeg().y2 + enemy.GetPos().y + enemy.GetHitBoxLeg().y1) / 2, 0.0f);
			width = std::abs(attackPos.x - enemyLeg.x);
			height = std::abs(attackPos.y - enemyLeg.y);

			//X�����̍U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxLeg().x2 - enemy.GetHitBoxLeg().x1) / 2) &&
				//Y�����U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxLeg().y2 - enemy.GetHitBoxLeg().y1) / 2)) return true;
		}
		
	}
	else
	{
		//�U������̒��S�����߂�
		Vector3 attackPos((m_pos.x - m_hitBoxAttack.x2 + m_pos.x - m_hitBoxAttack.x1) / 2,
			(m_pos.y + m_hitBoxAttack.y2 + m_pos.y + m_hitBoxAttack.y1) / 2, 0.0f);

		//��
		if (enemy.GetHitBoxHead().x1 != 0 ||
			enemy.GetHitBoxHead().y1 != 0 ||
			enemy.GetHitBoxHead().x2 != 0 ||
			enemy.GetHitBoxHead().y2 != 0)
		{
			//�G�̓��̂��ꔻ��////////////////////////////////////////
			Vector3 enemyHead((enemy.GetPos().x + enemy.GetHitBoxHead().x2 + enemy.GetPos().x + enemy.GetHitBoxHead().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxHead().y2 + enemy.GetPos().y + enemy.GetHitBoxHead().y1) / 2, 0.0f);

			width = std::abs(attackPos.x - enemyHead.x);
			height = std::abs(attackPos.y - enemyHead.y);

			//X�����̍U���̕��Ɠ��̕��𑫂����l��菬�����Ȃ瓖�����Ă�
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxHead().x2 - enemy.GetHitBoxHead().x1) / 2) &&
				//X�����̍U���̕��Ɠ��̕��𑫂����l��菬�����Ȃ瓖�����Ă�
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxHead().y2 - enemy.GetHitBoxHead().y1) / 2)) return true;
			////////////////////////////////////////////////////////////
		}
	

		//��
		if (enemy.GetHitBoxBody().x1 != 0 ||
			enemy.GetHitBoxBody().y1 != 0 ||
			enemy.GetHitBoxBody().x2 != 0 ||
			enemy.GetHitBoxBody().y2 != 0)
		{
			//�G�̑̂̂��ꔻ��///////////////////////////////////////
			Vector3 enemyBody((enemy.GetPos().x + enemy.GetHitBoxBody().x2 + enemy.GetPos().x + enemy.GetHitBoxBody().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxBody().y2 + enemy.GetPos().y + enemy.GetHitBoxBody().y1) / 2, 0.0f);

			width = std::abs(attackPos.x - enemyBody.x);
			height = std::abs(attackPos.y - enemyBody.y);

			//X�����̍U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxBody().x2 - enemy.GetHitBoxBody().x1) / 2) &&
				//Y�����U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxBody().y2 - enemy.GetHitBoxBody().y1) / 2)) return true;
			/////////////////////////////////////////////////////////
		}
	


		//��
		if (enemy.GetHitBoxLeg().x1 != 0 ||
			enemy.GetHitBoxLeg().y1 != 0 ||
			enemy.GetHitBoxLeg().x2 != 0 ||
			enemy.GetHitBoxLeg().y2 != 0)
		{
			//�G�̑��̂��ꔻ��
			Vector3 enemyLeg((enemy.GetPos().x + enemy.GetHitBoxLeg().x2 + enemy.GetPos().x + enemy.GetHitBoxLeg().x1) / 2,
				(enemy.GetPos().y + enemy.GetHitBoxLeg().y2 + enemy.GetPos().y + enemy.GetHitBoxLeg().y1) / 2, 0.0f);
			width = std::abs(attackPos.x - enemyLeg.x);
			height = std::abs(attackPos.y - enemyLeg.y);

			//X�����̍U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
			if (width < std::abs((m_hitBoxAttack.x2 - m_hitBoxAttack.x1) / 2) + std::abs((enemy.GetHitBoxLeg().x2 - enemy.GetHitBoxLeg().x1) / 2) &&
				//Y�����U���̕��Ƒ̂̕��𑫂����l��菬�����Ȃ瓖�����Ă�
				height < std::abs((m_hitBoxAttack.y2 - m_hitBoxAttack.y1) / 2) + std::abs((enemy.GetHitBoxLeg().y2 - enemy.GetHitBoxLeg().y1) / 2)) return true;
		}
	
	}



	return false;
}

bool Bullet::HitCheckBullet(Bullet& otherBullet)
{

	//X�AY�����̗���Ă��鋗�����Βl�����Ď擾
	float width = std::abs(m_pos.x - otherBullet.GetPos().x);
	float height = std::abs(m_pos.y - otherBullet.GetPos().y);

	//X�����̋������L�����̕����傫����Γ������Ă��Ȃ�
	if (width > std::abs(GetHitBoxAttack().x1 - otherBullet.GetHitBoxAttack().x2))	return false;
	//Y�����̋������L�����̍������傫����Γ������Ă��Ȃ�
	if (height > std::abs(GetHitBoxAttack().y1 - otherBullet.GetHitBoxAttack().y2))	return false;

	//�����܂ł���Γ������Ă�
	return true;
}

void Bullet::InitHitBoxAttack()
{
	//�U���͈�
	m_hitBoxAttack.x1 = -kHadouRadius;
	m_hitBoxAttack.y1 = -kHadouRadius;
	m_hitBoxAttack.x2 = kHadouRadius;
	m_hitBoxAttack.y2 = kHadouRadius;
}

