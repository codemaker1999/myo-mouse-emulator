/*

 ~~__ ____   v   v           \_|_/
   ||/    \    v           __/o o\__
   |/      \       (@@@@)    \_O_/
   /        \     (@@@@@@)   / | \
  /   MATH   \    (@@@@@@)
  ------------     (@@@@)
  | __     __|       ||
  ||#| __ |#||       ||
~~|----||----|~~~~~~~||~~~~~~
       ##           *   *
*    * ##   *  *      *
        ##         *
*  *    ## *           *
*        ##     *
          ##
*/

class VectorMath
{
public:
    float dot(myo::Vector3<float> vec1, myo::Vector3<float> vec2)
    {
        return vec1.x()*vec2.x() + vec1.y()*vec2.y() + vec1.z()*vec2.z();
    }

    myo::Vector3<float> scale(float c, myo::Vector3<float> vec)
    {
        myo::Vector3<float> scaledVec(c*vec.x(), c*vec.y(), c*vec.z());
        return scaledVec;
    }

    myo::Vector3<float> subtract(myo::Vector3<float> vec1, myo::Vector3<float> vec2)
    {
        myo::Vector3<float> newVec(vec1.x() - vec2.x(), vec1.y() - vec2.y(), vec1.z() - vec2.z());
        return newVec;
    }

    myo::Vector3<float> cross(myo::Vector3<float> vec1, myo::Vector3<float> vec2)
    {
        float x1 = vec1.x(); float y1 = vec1.y(); float z1 = vec1.z();
        float x2 = vec2.x(); float y2 = vec2.y(); float z2 = vec2.z();
        myo::Vector3<float> newVec(
            y1*z2 - z1*y2,
            z1*x2 - x1*z2,
            x1*y2 - y1*x2
            );
        return newVec;
    }

    float norm(myo::Vector3<float> vec)
    {
        return sqrt(pow(vec.x(), 2) + pow(vec.y(), 2) + pow(vec.z(), 2));
    }

    myo::Vector3<float> proj(myo::Vector3<float> vec1, myo::Vector3<float> vec2)
    {
        return scale(dot(vec1, vec2) / pow(norm(vec2), 2), vec2);
    }

    myo::Vector3<float> normalize(myo::Vector3<float> vec)
    {
        return scale(1.0f / norm(vec), vec);
    }

    myo::Vector3<float> closestOrthogonalVector(myo::Vector3<float> point, myo::Vector3<float> normalVec)
    {
        // this is unused with current implementation. It can be used to construct an orthogonal basis for
        // arm movement, but non-orthogonal basis seems to work quite well.
        float x = normalVec.x(); float y = normalVec.y(); float z = normalVec.z();
        myo::Vector3<float> vecOnPlane(y*z, x*z, -2.0f*x*y); // dot product with normalVec is zero by construciton
        return subtract(point, proj(subtract(point, vecOnPlane), normalVec));
    }
};
