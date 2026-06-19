#include "GameManager.h"
#include "include.h"

#include "SampleScene.h"
#include "SceneManager.h"
#include "Profiler.h"

#undef main


int main(int argc, char** argv)
{ 
    Matrix3x3 m1;
    m1[0][0] = 1.0f; m1[1][0] = 0.0f; m1[2][0] = 2.0f;
    m1[0][1] = 0.0f; m1[1][1] = -1.0f; m1[2][1] = 1.0f;
    m1[0][2] = 1.0f; m1[1][2] = -2.0f; m1[2][2] = 0.0f;

    Matrix3x3 m2 = m1.Inverse();

    DEBUG_INFO << m1;
    DEBUG_INFO << ENDL;
    DEBUG_INFO << "Determinant : " << m1.determinant() << ENDL;
    DEBUG_INFO << ENDL;
    DEBUG_INFO << m2;
    DEBUG_INFO << ENDL;
    DEBUG_INFO << m1 * m2;

    return 1;

    GameManager& gm = GameManager::GetInstance();

    gm.Init(640, 480);

    SceneManager::GetInstance().CreateScene<SampleScene>("SampleScene");
    SceneManager::GetInstance().CreateScene<SampleScene>("SampleScene2");

    SceneManager::GetInstance().SetCurrentSceneWithTag("SampleScene");
    SceneManager::GetInstance().SetCurrentSceneWithTag("SampleScene2");

    
    gm.Loop();
    gm.Close();

    return 0;
}