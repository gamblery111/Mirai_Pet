#pragma once


#include <memory>
#include <string>

namespace saba {
    class MMDModel;
    class VMDAnimation;
}

class PetModelController
{
private:
    std::shared_ptr<saba::MMDModel> m_model;
    std::unique_ptr<saba::VMDAnimation> m_vmd;
    float m_elapsed{ 0.0f };

public:
    PetModelController();
    ~PetModelController();

    bool LoadModel(const std::string& filepath, const std::string& mmdDataDir = "");
    bool LoadMotion(const std::string& path);

    void Update(float deltaSeconds); 

    // 给渲染器用的只读接口
    const saba::MMDModel* GetModel() const { return m_model.get(); }
    float GetElapsedTime() const { return m_elapsed; }
};