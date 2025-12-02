#include "PetModelController.h"

#include <saba/Model/MMD/PMXModel.h>
#include <saba/Model/MMD/PMDModel.h>
#include <saba/Model/MMD/VMDAnimation.h>
#include <saba/Base/Path.h>
#include <saba/Base/File.h>

PetModelController::PetModelController()
{
}

PetModelController::~PetModelController() = default;

bool PetModelController::LoadModel(const std::string& filepath, const std::string& mmdDataDir)
{
	std::string ext = saba::PathUtil::GetExt(filepath);
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	if (ext == "pmx")
	{
		auto pmx = std::make_shared<saba::PMXModel>();
		if (!pmx->Load(filepath, mmdDataDir))
			return false;
		m_model = pmx;
	}
	else if (ext == "pmd")
	{
		auto pmd = std::make_shared<saba::PMDModel>();
		if (!pmd->Load(filepath, mmdDataDir))
			return false;
		m_model = pmd;
	}
	else
	{
		return false;
	}

	return true;
}

bool PetModelController::LoadMotion(const std::string& path)
{
	if (!m_model) return false;

	m_vmd = std::make_unique<saba::VMDAnimation>();
	if (!m_vmd->Create(m_model))
		return false;

	saba::VMDFile vmdFile;
	if (!saba::ReadVMDFile(&vmdFile, path.c_str()))
		return false;

	m_vmd->Add(vmdFile);
	return true;
}

void PetModelController::Update(float deltaSeconds)
{
	if(!m_model || !m_vmd) return;

	m_elapsed += deltaSeconds;

	m_model->BeginAnimation();
	m_vmd->Evaluate(m_elapsed * 30.0f);   // MMD 标准 30fps
	m_model->UpdateAllAnimation(m_vmd.get(), m_elapsed * 30.0f, deltaSeconds);
	m_model->EndAnimation();

	m_model->Update();
}
