#ifndef PASSIVEWALKERSCENE_H
#define PASSIVEWALKERSCENE_H

#include "scene.h"

/**
 * @brief The PassiveWalkerScene class
 */
class PassiveWalkerScene : public Scene
{
    // Initial conditions from pwnCfg.m. We will need to try some alternative
    // initial conditions to evaluate stability.
    typedef struct InitialConditions {
        double l_s, l_t;    // length of shank and thigh
        double L;           // total leg length = length_shank+length_thigh
        double q[3];        // Three state variables (angles)
        double dq[3];       // Angular velocity of state variables
        double m_h;         // Mass of the hips
        double m_t;         // Mass of the thigh
        double m_s;         // Mass of the shank
        double a_1, b_1;    // Center of mass of thigh, adds up to l_t
        double a_2, b_2;    // Center of mass of shank, adds up to l_s
        double slope;       // Angle of the slope

        InitialConditions() {
            m_h = 1.0;
            m_t = 1.0;
            m_s = 0.2;
            l_s = 3.0;
            l_t = 3.0;
            L = l_s + l_t;
            q[0] = 0.163; q[1] = q[2] = -0.2837;
            dq[0] = -0.9717; dq[1] = dq[2] = 0.163;
            a_1 = 0.375;
            b_1 = l_t - a_1;
            a_2 = 0.175;
            b_2 = l_s - a_2;
            slope = -0.05;
        }
    } InitialConditions;

public:
    /// Construct a new passive walker
    PassiveWalkerScene();

    /// Move this somewhere useful when you have a class for e.g. actuators
    void applyHingeTorque(btHingeConstraint *pHinge, btScalar T);
};

#endif // PASSIVEWALKERSCENE_H
