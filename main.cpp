#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <myo/myo.hpp>

#include <io.h>

#include "win-mouse.cpp"
MouseController mCtrl;

#include "vec-math.cpp"
VectorMath v;

int setup_stage;
bool wait;
uint64_t t;
bool poseLock;
bool firstClick;
uint64_t lastLeftClick;

int mouse_x;
int mouse_y;

// calibration vectors
myo::Vector3<float> vec_center(0, 0, 1);
myo::Vector3<float> vec_up(0, 0, 1);
myo::Vector3<float> vec_left(0, 0, 1);

void maybeDragMouse(float x, float y)
{
    int dist_x = -x * 15;
    int dist_y = -y * 15;
    mouse_x = mouse_x + dist_x;
    mouse_x = mouse_x > mCtrl.SCREENWIDTH ? mCtrl.SCREENWIDTH : mouse_x;
    mouse_x = mouse_x < 0 ? 0 : mouse_x;
    mouse_y = mouse_y + dist_y;
    mouse_y = mouse_y > mCtrl.SCREENHEIGHT ? mCtrl.SCREENHEIGHT : mouse_y;
    mouse_y = mouse_y < 0 ? 0 : mouse_y;
    mCtrl.mouseMove(mouse_x, mouse_y);
}

/*
    ~~~__ __   v   v           \_|_/
       ||/  \    v           __/o o\__
       |/    \       (@@@@)    \_v_/
       /      \     (@@@@@@)   / | \
      /  MYO   \    (@@@@@@)
      ----------     (@@@@)
      | __   __|       ||
      ||#|__|#||       ||
~~~~~~|---||---|~~~~~~~||~~~~~~
          ##           *   *
*    *    ##   *  *      *
         ##         *
*  *    ## *           *
*       ##     *
        ##
*/

class DataCollector : public myo::DeviceListener {
public:
    DataCollector()
    {
    }
    // onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
    // as a unit quaternion.
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        // wait flag tells us we must wait for user
        if (wait) {
            if (timestamp > t) {
                wait = false;
            }
        }
        else if (setup_stage == 4){
            // initialization complete
            myo::Vector3<float> vec(1, 0, 0);
            vec = myo::rotate(quat, vec);
            float x = v.dot(v.proj(vec, vec_left), vec_left);
            float y = v.dot(v.proj(vec, vec_up), vec_up);
            maybeDragMouse(x, y);
        }
        else if (setup_stage == 3){
            // create coordinate system
            printf("vec_center   = (%f,%f,%f)\n", vec_center.x(), vec_center.y(), vec_center.z());
            printf("rough vec_up = (%f,%f,%f)\n", vec_up.x(), vec_up.y(), vec_up.z());
            // make vec_up orthogonal to vec_center
            //vec_up = vecNormalize(closestOrthogonalVector(vec_up, vec_center));
            //printf("clean vec_up = (%f,%f,%f)\n", vec_up.x(), vec_up.y(), vec_up.z());
            // left side of person wearing the Myo
            vec_left = v.cross(vec_center, vec_up);
            printf("vec_left     = (%f,%f,%f)\n", vec_left.x(), vec_left.y(), vec_left.z());
            printf("vec_up   . vec_center = %f\n", v.dot(vec_up, vec_center));
            printf("vec_up   . vec_left   = %f\n", v.dot(vec_up, vec_left));
            printf("vec_left . vec_center = %f\n", v.dot(vec_left, vec_center));
            setup_stage++;
        }
        else if (setup_stage == 2) {
            // measure vec_up
            myo::Vector3<float> vec(1, 0, 0);
            vec_center = myo::rotate(quat, vec);
            printf("Measured.\n");
            setup_stage++;
        }
        else if (setup_stage == 1) {
            // measure vec_center
            myo::Vector3<float> vec(1, 0, 0);
            vec_up = myo::rotate(quat, vec);
            printf("Measured.\n");
            printf("Point at center of screen. Measuring in 3 seconds.\n");
            wait = true;
            t = timestamp + 3000000;
            setup_stage++;
        }
        else if (setup_stage == 0) {
            // waiting for user to be ready
            printf("Point straight up. Measuring in 3 seconds.\n");
            wait = true;
            t = timestamp + 3000000;
            setup_stage++;
        }
    }

    // onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
    // making a fist, or not making a fist anymore.
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        if (pose == myo::Pose::fist) {
            mCtrl.mouseLeftDown();
        }
        else if (pose == myo::Pose::waveIn) {
            if (!firstClick){
                mCtrl.mouseLeftClick();
                lastLeftClick = timestamp;
            }
            else if ( timestamp > lastLeftClick + 300000 ) {
                mCtrl.mouseLeftClick();
                lastLeftClick = timestamp;
            }
        }
        else if (pose == myo::Pose::waveOut) {
            //mCtrl.mouseLeftUp();
            //mCtrl.mouseRightDown();
        }
        else if (pose == myo::Pose::fingersSpread) {
            mCtrl.mouseLeftUp();
            //mCtrl.mouseRightUp();
        }
        else if (pose == myo::Pose::rest) {
            mCtrl.mouseLeftUp();
            //mCtrl.mouseRightUp();
        }
    }
};

/*

 ~~__ ____   v   v           \_|_/
   ||/    \    v           __/o o\__
   |/      \       (@@@@)    \_x_/
   /        \     (@@@@@@)   / | \
  /   MAIN   \    (@@@@@@)
  ------------     (@@@@)
  | __     __|       ||
  ||#| __ |#||       ||
~~|----||----|~~~~~~~||~~~~~~
       ##           *   *
*    *  ##   *  *      *
        ##         *
*  *    ## *           *
*        ##     *
         ##
*/

int main(int argc, char** argv)
{
    // constants
    mouse_x = ((int)mCtrl.SCREENWIDTH / 2.0);
    mouse_y = ((int)mCtrl.SCREENHEIGHT / 2.0);

    /*
    setup_stage == 0 : waiting for user to be ready
    setup_stage == 1 : measure vec_center
    setup_stage == 2 : measure vec_up
    setup_stage == 3 : create coordinate system
    setup_stage == 4 : initialization complete
    */
    int setup_stage = 0;
    bool wait = false;
    bool poseLock = false;
    bool firstClick = false;

    // run
    try {
        myo::Hub hub;
        std::cout << "Attempting to find a Myo..." << std::endl;
        myo::Myo* myo = hub.waitForAnyMyo(10000);
        if (!myo) {
            throw std::runtime_error("Unable to find a Myo!");
        }
        std::cout << "Connected to " << myo->macAddressAsString() << "." << std::endl << std::endl;
        if (!myo->isTrained()) {
            throw std::runtime_error("Myo has not been trained. Do you need to run the trainer?");
        }
        DataCollector collector;
        hub.addListener(&collector);
        while (1) {
            hub.run(1000 / 20);
        }
        std::cerr << "Press enter to quit.";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();
        return 1;
    }

}