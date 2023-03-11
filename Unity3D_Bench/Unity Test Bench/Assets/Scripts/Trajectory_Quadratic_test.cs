using QuadRegressionTesting;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Trajectory_Quadratic_test : MonoBehaviour
{
    public struct HitPoint {
        public Vector3 hitLocalPosition;
        public Vector3 hitGlobalPosition;
        public Vector3 PlaneGlobalPosition;
        public Vector3 PlaneHitNormal;
        public Vector3 PointHitNormal;
        public float HitTime;
    }


    public Transform plane;
    public Transform endPlaneOrientation;
    public Transform sourceSphere;

    public GameObject prefab_sphere;
    public GameObject prefab_capsule;
    public GameObject prefab_quad;
    public Transform secondSphere;
    public Transform thirdSphere;

    System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();

    // Start is called before the first frame update
    void Start()
    {
        float velocity = 5;

        /*double a = 0.203736060367116;
        double b = -1.52690864140882;
        double c = -1.52690864140882;
        double t1 = 1.0;
        double t2 = 2.0;*/

        /*float a = 1;
        float b = 0;
        float c = 0;

        const int SIZE = 100000;

        float[] values = new float[SIZE];

        System.Diagnostics.Stopwatch p_watch = new System.Diagnostics.Stopwatch();

        p_watch.Start();
        for (int i = 0; i < SIZE; i++)
        {
            values[i] = QuadraticArcLength2(a + (i / (float)SIZE), (i / (float)SIZE), -(i / (float)SIZE), 0, 1, true);
        }
        p_watch.Stop();

        Debug.LogFormat("Integral Time: {0}", p_watch.Elapsed.TotalSeconds);

        values = new float[SIZE];

        p_watch.Restart();
        for (int i = 0; i < SIZE; i++)
        {
            values[i] = QuadraticArcLengthEstimate(a + (i / (float)SIZE), (i / (float)SIZE), -(i / (float)SIZE), 0, 1, 1);
        }
        p_watch.Stop();

        Debug.LogFormat("Estimate Time: {0}", p_watch.Elapsed.TotalSeconds);*/



        //Debug.LogFormat("1: {0}", QuadraticArcLength2(a, b, c, t1, t2));
        //Debug.LogFormat("2: {0}", QuadraticArcLengthEstimate(a, b, c, t1, t2, 10000000));

        //Debug.LogFormat("1: {0}", QuadraticArcLength(a, b, c, t1, t2));
        //Debug.LogFormat("2: {0}", QuadraticArcLength(a, b, c, t2));
        //Debug.LogFormat("3: {0}", QuadraticArcLengthEstimate(a, b, c, t1, t2, 10000000));

        watch.Start();


        benchTestLocalize(
            plane.position,
            plane.forward,
            endPlaneOrientation.position,
            endPlaneOrientation.forward,
            sourceSphere.position,
            sourceSphere.position + sourceSphere.forward * velocity,
            1f
        );
    }

    // Update is called once per frame
    void Update()
    {
        return;

        if (watch.Elapsed.TotalSeconds > 0.1f)
            watch.Restart();
        else
            return;

        float co_range = 1;
        float range = 0.01f;

        float a = UnityEngine.Random.Range(-co_range, co_range);
        float b = UnityEngine.Random.Range(-co_range, co_range);
        float c = UnityEngine.Random.Range(-co_range, co_range);
        float t1 = UnityEngine.Random.Range(-range, range);
        float t2 = t1 + UnityEngine.Random.Range(1f * range, 5f * range);

        float r1 = QuadraticArcLength2(a, b, c, t1, t2, false);
        float r2 = QuadraticArcLengthEstimate(a, b, c, t1, t2, 100);

        float diff = Mathf.Abs(r1 - r2);
        if (diff > range / 10)
        {
            Debug.LogFormat("{0}x^2 + {1}x + {2}: {3} == {4}: {5}", a, b, c, r1, r2, diff);
        }

        
    }

    void PlaceSpheresInGlobalSpace()
    {
        float velocity = 1;
        Vector3 dir = new Vector3(1, 0, 0);

        Vector3 firstPosition = sourceSphere.transform.position + (dir * (velocity / 2));

        Vector3 secondPosition = sourceSphere.transform.position + (dir * (velocity));

        GameObject second = GameObject.Instantiate(sourceSphere.gameObject, firstPosition, Quaternion.identity, null);
        secondSphere = second.transform;

        GameObject third = GameObject.Instantiate(sourceSphere.gameObject, secondPosition, Quaternion.identity, null);
        thirdSphere = third.transform;

    }

    void benchTestLocalize(
        Vector3 planePositionStart,
        Vector3 planeDirectionStart,
        Vector3 planePositionEnd,
        Vector3 planeDirectionEnd,
        Vector3 sphereStart,
        Vector3 sphereFinish,
        float time)
    {
        const int TEST_SIZE = 50000;

        HitPoint[] hits = new HitPoint[TEST_SIZE];

        System.Diagnostics.Stopwatch p_watch = new System.Diagnostics.Stopwatch();

        p_watch.Start();
        for (int i = 0; i < TEST_SIZE; i++)
        {
            sphereStart = new Vector3(sphereStart.x += (1 / TEST_SIZE), sphereStart.y, sphereStart.z);
            hits[i] = QuadraticHit(planePositionStart, planeDirectionStart, planePositionEnd, planeDirectionEnd, sphereStart, sphereFinish, time);
        }
        p_watch.Stop();

        Debug.LogFormat("benchTestLocalize Time: {0}", p_watch.Elapsed.TotalSeconds);
    }

    void test_localize2(
        Vector3 planePositionStart,
        Vector3 planeDirectionStart,
        Vector3 planePositionEnd,
        Vector3 planeDirectionEnd,
        Vector3 sphereStart,
        Vector3 sphereFinish,
        float time)
    {
        Vector3[] localPositions = LocalizePath(planePositionStart, planeDirectionStart, planePositionEnd, planeDirectionEnd, sphereStart, sphereFinish, time);
        HitPoint hit = QuadraticHit(planePositionStart, planeDirectionStart, planePositionEnd, planeDirectionEnd, sphereStart, sphereFinish, time);

        plane.gameObject.name = "p1";
        sourceSphere.gameObject.name = "c1";

        Vector3 firstLocalPosition = localPositions[0];
        Vector3 secondLocalPosition = localPositions[1];
        Vector3 thirdLocalPosition = localPositions[2];

        GameObject.Instantiate(prefab_sphere, firstLocalPosition, Quaternion.identity).name = "1";
        GameObject.Instantiate(prefab_sphere, secondLocalPosition, Quaternion.identity).name = "2";
        GameObject.Instantiate(prefab_sphere, thirdLocalPosition, Quaternion.identity).name = "3";

        GameObject hit_point = GameObject.Instantiate(prefab_sphere, hit.hitLocalPosition, Quaternion.identity);
        hit_point.name = "hit point";
        hit_point.transform.localScale = new Vector3(0.1f, 0.1f, 0.1f);

        GameObject midPlane = GameObject.Instantiate(prefab_quad, hit.PlaneGlobalPosition, Quaternion.identity);
        midPlane.name = "plane hit";
        midPlane.transform.forward = hit.PlaneHitNormal;

        Debug.DrawRay(hit.hitGlobalPosition, hit.PlaneHitNormal, Color.green, 5000);
    }

    void test_localize (
        Vector3 planePositionStart, 
        Vector3 planeDirectionStart, 
        Vector3 planePositionEnd, 
        Vector3 planeDirectionEnd, 
        Vector3 sphereStart, 
        Vector3 sphereFinish, 
        float time)
    {

        // Transform path of sphere to the plane's local space.
        Vector3[] localPositions = LocalizePath(planePositionStart, planeDirectionStart, planePositionEnd, planeDirectionEnd, sphereStart, sphereFinish, time);

        plane.gameObject.name = "p1";
        sourceSphere.gameObject.name = "c1";

        Vector3 firstLocalPosition = localPositions[0];

        //Vector3 planeMidPos = Vector3.Lerp(planePositionStart, planePositionEnd, 0.5f);
        //Vector3 planeMidDir = Vector3.Lerp(planeDirectionStart, planeDirectionEnd, 0.5f);


        //GameObject midPlane = GameObject.Instantiate(prefab_quad, planeMidPos, Quaternion.identity);
        //midPlane.name = "p2";
        //midPlane.transform.forward = planeMidDir;

        
        Vector3 secondLocalPosition = localPositions[1];

        //Vector3 secondGlobalPos = Vector3.Lerp(sphereStart, sphereFinish, 0.5f);
        //GameObject.Instantiate(prefab_capsule, secondGlobalPos, Quaternion.identity).name = "c2";



        //GameObject endPlane = GameObject.Instantiate(prefab_quad, planePositionEnd, Quaternion.identity);
        //endPlane.name = "p3";
        //endPlane.transform.forward = planeDirectionEnd;



        Vector3 thirdLocalPosition = localPositions[2];
        //GameObject.Instantiate(prefab_capsule, sphereFinish, Quaternion.identity).name = "c3";

        GameObject.Instantiate(prefab_sphere, firstLocalPosition, Quaternion.identity).name = "1";
        GameObject.Instantiate(prefab_sphere, secondLocalPosition, Quaternion.identity).name = "2";
        GameObject.Instantiate(prefab_sphere, thirdLocalPosition, Quaternion.identity).name = "3";



        /*Vector3 globalPlaneToCapsule1 = plane.transform.position - sphereStart;
        Vector3 localPlaneToSphere1 = Vector3.zero - firstLocalPosition;
        Vector3 globalPlaneToCapsuleDir1 = globalPlaneToCapsule1.normalized;
        Vector3 localPlaneToSphereDir1 = localPlaneToSphere1.normalized;
        float globalDistance1 = Vector3.Distance(plane.transform.position, sphereStart);
        float localDistance1 = Vector3.Distance(Vector3.zero, firstLocalPosition);
        Debug.Log(
            "Plane to Sphere1: " + globalPlaneToCapsule1 + 
            ", local: " + localPlaneToSphere1 + 
            ", global dir: " + globalPlaneToCapsuleDir1 + 
            ", local dir: " + localPlaneToSphereDir1 + 
            ", globalDist: " + globalDistance1 + 
            ", localDist: " + localDistance1);

        Vector3 globalPlaneToCapsule2 = midPlane.transform.position - secondGlobalPos;
        Vector3 localPlaneToSphere2 = Vector3.zero - secondLocalPosition;
        Vector3 globalPlaneToCapsuleDir2 = globalPlaneToCapsule2.normalized;
        Vector3 localPlaneToSphereDir2 = localPlaneToSphere2.normalized;
        float globalDistance2 = Vector3.Distance(midPlane.transform.position, secondGlobalPos);
        float localDistance2 = Vector3.Distance(Vector3.zero, secondLocalPosition);
        Debug.Log(
            "Plane to Sphere2: " + globalPlaneToCapsule2 + 
            ", local: " + localPlaneToSphere2 + 
            ", global dir: " + globalPlaneToCapsuleDir2 + 
            ", local dir: " + localPlaneToSphereDir2 +
            ", globalDist: " + globalDistance2 +
            ", localDist: " + localDistance2);

        Vector3 globalPlaneToCapsule3 = endPlane.transform.position - sphereFinish;
        Vector3 localPlaneToSphere3 = Vector3.zero - thirdLocalPosition;
        Vector3 globalPlaneToCapsuleDir3 = globalPlaneToCapsule1.normalized;
        Vector3 localPlaneToSphereDir3 = localPlaneToSphere1.normalized;
        float globalDistance3 = Vector3.Distance(endPlane.transform.position, sphereFinish);
        float localDistance3 = Vector3.Distance(Vector3.zero, thirdLocalPosition);
        Debug.Log(
            "Plane to Sphere3: " + globalPlaneToCapsule3 + 
            ", local: " + localPlaneToSphere3 + 
            ", global dir: " + globalPlaneToCapsuleDir3 + 
            ", local dir: " + localPlaneToSphereDir3 +
            ", globalDist: " + globalDistance3 +
            ", localDist: " + localDistance3);*/

        Debug.Log(localPositions[0]);
        Debug.Log(localPositions[1]);
        Debug.Log(localPositions[2]);

        Vector2[] ZY_localPos = new Vector2[3];
        ZY_localPos[0] = new Vector2(localPositions[0].z, localPositions[0].y);
        ZY_localPos[1] = new Vector2(localPositions[1].z, localPositions[1].y);
        ZY_localPos[2] = new Vector2(localPositions[2].z, localPositions[2].y);

        Vector2[] ZX_localPos = new Vector2[3];
        ZX_localPos[0] = new Vector2(localPositions[0].z, localPositions[0].x);
        ZX_localPos[1] = new Vector2(localPositions[1].z, localPositions[1].x);
        ZX_localPos[2] = new Vector2(localPositions[2].z, localPositions[2].x);

        bool is_continous_right = ZY_localPos[0].x < ZY_localPos[1].x && ZY_localPos[1].x < ZY_localPos[2].x;
        bool is_continous_left = ZY_localPos[0].x > ZY_localPos[1].x && ZY_localPos[1].x > ZY_localPos[2].x;
        bool is_continous = is_continous_right || is_continous_left;

        bool z_is_y = false;
        if (!is_continous)
        {
            Vector2[] tmp_vec = ZY_localPos;
            ZY_localPos[0] = new Vector3(tmp_vec[0].y, tmp_vec[0].x);
            ZY_localPos[1] = new Vector3(tmp_vec[1].y, tmp_vec[1].x);
            ZY_localPos[2] = new Vector3(tmp_vec[2].y, tmp_vec[2].x);

            tmp_vec = ZX_localPos;
            ZX_localPos[0] = new Vector3(tmp_vec[0].y, tmp_vec[0].x);
            ZX_localPos[1] = new Vector3(tmp_vec[1].y, tmp_vec[1].x);
            ZX_localPos[2] = new Vector3(tmp_vec[2].y, tmp_vec[2].x);
            z_is_y = true;
        }


        // Create quadratic Regressions
        float[] zy_coef = Regression(ZY_localPos);
        float[] zx_coef = Regression(ZX_localPos);

        Debug.LogFormat("{0}x^2 + {1}x + {2}", zy_coef[0], zy_coef[1], zy_coef[2]);

        //float arcLenInteg = QuadraticArcLength(zy_coef[0], zy_coef[1], zy_coef[2], localPositions[0].z, localPositions[2].z);

        float curve_start_zy = ZY_localPos[0].x;
        float curve_finish_zy = 0;

        float curve_start_zx = ZX_localPos[0].x;
        float curve_finish_zx = 0;

        float hit_location_x = 0;
        float hit_location_y = 0;

        if (z_is_y)
        {
            float[] ans_zy = QuadraticFormula(zy_coef[0], zy_coef[1], zy_coef[2]);
            Debug.LogFormat("ZY ANS: {0}, {1}", ans_zy[0], ans_zy[1]);

            curve_finish_zy = Mathf.Max(ans_zy[0], ans_zy[1]);

            if (ZY_localPos[2].x < ZY_localPos[0].x)
            {
                curve_finish_zy = Mathf.Min(ans_zy[0], ans_zy[1]);
            }



            float[] ans_zx = QuadraticFormula(zx_coef[0], zx_coef[1], zx_coef[2]);
            Debug.LogFormat("ZX ANS: {0}, {1}", ans_zx[0], ans_zx[1]);

            curve_finish_zx = Mathf.Max(ans_zx[0], ans_zx[1]);

            if (ZX_localPos[2].x < ZX_localPos[0].x)
            {
                curve_finish_zx = Mathf.Min(ans_zx[0], ans_zx[1]);
            }

            hit_location_x = curve_finish_zx;
            hit_location_y = curve_finish_zy;

            Debug.LogFormat("ZY Final answer: {0}", curve_finish_zy);
            Debug.LogFormat("ZX Final answer: {0}", curve_finish_zx);

            GameObject hit_point = GameObject.Instantiate(prefab_sphere, new Vector3(hit_location_x, hit_location_y, 0), Quaternion.identity);
            hit_point.name = "hit point";
            hit_point.transform.localScale = new Vector3(0.1f, 0.1f, 0.1f);
        }
        else
        {
            hit_location_y = (float)QuadraticSolveY(zy_coef[0], zy_coef[1], zy_coef[2], 0);
            Debug.LogFormat("ZY ANS: {0}", hit_location_y);

            hit_location_x = (float)QuadraticSolveY(zx_coef[0], zx_coef[1], zx_coef[2], 0);
            Debug.LogFormat("ZX ANS: {0}", hit_location_x);

            GameObject hit_point = GameObject.Instantiate(prefab_sphere, new Vector3(hit_location_x, hit_location_y, 0), Quaternion.identity);
            hit_point.name = "hit point";
            hit_point.transform.localScale = new Vector3(0.1f, 0.1f, 0.1f);
        }

        float full_len_zy = GetArcLength(ZY_localPos[0].x, ZY_localPos[2].x, zy_coef);
        float full_len_zx = GetArcLength(ZX_localPos[0].x, ZX_localPos[2].x, zx_coef);

        float full_len = Mathf.Max(full_len_zy, full_len_zx);

        float len_zy = GetArcLength(curve_start_zy, curve_finish_zy, zy_coef);
        float len_zx = GetArcLength(curve_start_zx, curve_finish_zx, zx_coef);

        float len = Mathf.Max(len_zy, len_zx);

        float hit_t = len / full_len;

        Debug.LogFormat("Plot zy Start/Stop: {0}, {1}, Length: {2} / {3} = {4}", ZY_localPos[0].x, ZY_localPos[2].x, len_zy, full_len_zy, len_zy / full_len_zy);
        DebugPlot_ZY(zy_coef[0], zy_coef[1], zy_coef[2], ZY_localPos[0].x, ZY_localPos[2].x, 0, 100, z_is_y);


        Debug.LogFormat("Plot zx Start/Stop: {0}, {1}, Length: {2} / {3} = {4}", ZX_localPos[0].x, ZX_localPos[2].x, len_zx, full_len_zx, len_zx / full_len_zx);
        DebugPlot_ZX(zx_coef[0], zx_coef[1], zx_coef[2], ZX_localPos[0].x, ZX_localPos[2].x, 0, 100, z_is_y);



        Vector3 hitPlanePos = Vector3.Lerp(planePositionStart, planePositionEnd, hit_t);
        Vector3 hitPlaneDir = Vector3.Lerp(planeDirectionStart, planeDirectionEnd, hit_t);


        GameObject midPlane = GameObject.Instantiate(prefab_quad, hitPlanePos, Quaternion.identity);
        midPlane.name = "plane hit";
        midPlane.transform.forward = hitPlaneDir;

        Vector3 sphereLocalHitPos = new Vector3(hit_location_x, hit_location_y, 0);
        Vector3 sphereGlobalPos = TransformExtensions.TransformPointUnscaled(hitPlanePos, hitPlaneDir, sphereLocalHitPos);

        Debug.DrawRay(sphereGlobalPos, hitPlaneDir, Color.green, 5000);
    }


    HitPoint QuadraticHit(
        Vector3 planePositionStart,
        Vector3 planeDirectionStart,
        Vector3 planePositionEnd,
        Vector3 planeDirectionEnd,
        Vector3 sphereStart,
        Vector3 sphereFinish,
        float time)
    {
        // Transform point trajectory from two points local position 
        // relative to the plane center with an additional third point
        Vector3[] localPositions = LocalizePath(planePositionStart, planeDirectionStart, planePositionEnd, planeDirectionEnd, sphereStart, sphereFinish, time);


        // split each point position into two 2-Dimensional planes:
        // From the "Top" (Z and X axis) and from the "Side" (Z and Y axis)

        // TODO: The "front" side (Y and X) is not usefull for detecting if the
        // point crossed the collision barrier, however if it's the longest path
        // then it could be usefull to get the most accurate time.

        Vector2[] ZY_localPos = new Vector2[3];
        ZY_localPos[0] = new Vector2(localPositions[0].z, localPositions[0].y);
        ZY_localPos[1] = new Vector2(localPositions[1].z, localPositions[1].y);
        ZY_localPos[2] = new Vector2(localPositions[2].z, localPositions[2].y);

        Vector2[] ZX_localPos = new Vector2[3];
        ZX_localPos[0] = new Vector2(localPositions[0].z, localPositions[0].x);
        ZX_localPos[1] = new Vector2(localPositions[1].z, localPositions[1].x);
        ZX_localPos[2] = new Vector2(localPositions[2].z, localPositions[2].x);


        // Determine which axis should be used as the "X" axis in the quadratic 
        // equations below. As the Z axis is shared between the two planes, if
        // the points are not continuous along the z-axis then the x and y
        // should be swapped between the two planes.

        bool is_continous_right = ZY_localPos[0].x < ZY_localPos[1].x && ZY_localPos[1].x < ZY_localPos[2].x;
        bool is_continous_left = ZY_localPos[0].x > ZY_localPos[1].x && ZY_localPos[1].x > ZY_localPos[2].x;
        bool is_continous = is_continous_right || is_continous_left;

        bool z_is_y = false;
        if (!is_continous)
        {
            Vector2[] tmp_vec = ZY_localPos;
            ZY_localPos[0] = new Vector3(tmp_vec[0].y, tmp_vec[0].x);
            ZY_localPos[1] = new Vector3(tmp_vec[1].y, tmp_vec[1].x);
            ZY_localPos[2] = new Vector3(tmp_vec[2].y, tmp_vec[2].x);

            tmp_vec = ZX_localPos;
            ZX_localPos[0] = new Vector3(tmp_vec[0].y, tmp_vec[0].x);
            ZX_localPos[1] = new Vector3(tmp_vec[1].y, tmp_vec[1].x);
            ZX_localPos[2] = new Vector3(tmp_vec[2].y, tmp_vec[2].x);
            z_is_y = true;
        }


        // Create quadratic Regressions for both planes to get A, B and C constants.
        float[] zy_coef = Regression(ZY_localPos);
        float[] zx_coef = Regression(ZX_localPos);


        float curve_start_zy = ZY_localPos[0].x;
        float curve_finish_zy = 0;

        float curve_start_zx = ZX_localPos[0].x;
        float curve_finish_zx = 0;

        float hit_location_x = 0;
        float hit_location_y = 0;

        // Determine the X and Y positions of the hit by either
        // solving for X = 0 or Y = 0, depending on what the
        // collision axis is.

        if (z_is_y)
        {
            // If the axis X and Y was swapped, the X is the collision axis and we need to
            // determine where the function crosses the X axis using the quadratic formula
            // for x = 0. Because these functions cross the X axis in two points, we
            // need to determine which one of the two to use.

            float[] ans_zy = QuadraticFormula(zy_coef[0], zy_coef[1], zy_coef[2]);

            curve_finish_zy = Mathf.Max(ans_zy[0], ans_zy[1]);

            if (ZY_localPos[2].x < ZY_localPos[0].x)
            {
                curve_finish_zy = Mathf.Min(ans_zy[0], ans_zy[1]);
            }



            float[] ans_zx = QuadraticFormula(zx_coef[0], zx_coef[1], zx_coef[2]);

            curve_finish_zx = Mathf.Max(ans_zx[0], ans_zx[1]);

            if (ZX_localPos[2].x < ZX_localPos[0].x)
            {
                curve_finish_zx = Mathf.Min(ans_zx[0], ans_zx[1]);
            }


            hit_location_x = curve_finish_zx;
            hit_location_y = curve_finish_zy;
        }
        else
        {
            // If the axis wasn't swapped, we can just solve for Y by plugging
            // zero into the quadratic equation to get the position on each
            // plane.

            hit_location_y = (float)QuadraticSolveY(zy_coef[0], zy_coef[1], zy_coef[2], 0);
            hit_location_x = (float)QuadraticSolveY(zx_coef[0], zx_coef[1], zx_coef[2], 0);
        }

        // Get full arc length of the curve in each plane and compare it against the
        // length of the curve from the first point to the hit point on the
        // collision plane. Only the longest length is used. "hit_t" is a value
        // between 0 - 1 to denote the time between frames the collision most
        // likely occured.
        float full_len_zy = GetArcLength(ZY_localPos[0].x, ZY_localPos[2].x, zy_coef);
        float full_len_zx = GetArcLength(ZX_localPos[0].x, ZX_localPos[2].x, zx_coef);
        float full_len = Mathf.Max(full_len_zy, full_len_zx);

        float len_zy = GetArcLength(curve_start_zy, curve_finish_zy, zy_coef);
        float len_zx = GetArcLength(curve_start_zx, curve_finish_zx, zx_coef);
        float len = Mathf.Max(len_zy, len_zx);

        float hit_t = len / full_len;


        // Use hit time to interpolate plane position and direction between frames.

        Vector3 hitLocal = new Vector3(hit_location_x, hit_location_y, 0);
        Vector3 planeNormal = Vector3.Lerp(planeDirectionStart, planeDirectionEnd, hit_t);
        Vector3 PlanePos = Vector3.Lerp(planePositionStart, planePositionEnd, hit_t);
        Vector3 sphereGlobalPos = TransformExtensions.TransformPointUnscaled(PlanePos, planeNormal, hitLocal);

        HitPoint hit = default;
        hit.hitLocalPosition = hitLocal;
        hit.hitGlobalPosition = sphereGlobalPos;
        hit.PlaneGlobalPosition = PlanePos;
        hit.PlaneHitNormal = planeNormal;
        hit.PointHitNormal = hit.PlaneHitNormal; // TODO: calculate direction point hit plane from.
        hit.HitTime = hit_t;

        return hit;
    }


    Vector3[] LocalizePath(
        Vector3 planePositionStart,
        Vector3 planeDirectionStart,
        Vector3 planePositionEnd,
        Vector3 planeDirectionEnd,
        Vector3 sphereStart,
        Vector3 sphereFinish,
        float time)
    {
        Vector3[] res = new Vector3[3];

        res[0] = TransformExtensions.InverseTransformPointUnscaled(planePositionStart, planeDirectionStart, sphereStart);

        Vector3 planeMidPos = Vector3.Lerp(planePositionStart, planePositionEnd, 0.5f);
        Vector3 planeMidDir = Vector3.Lerp(planeDirectionStart, planeDirectionEnd, 0.5f);
        Vector3 secondGlobalPos = Vector3.Lerp(sphereStart, sphereFinish, 0.5f);

        res[1] = TransformExtensions.InverseTransformPointUnscaled(planeMidPos, planeMidDir, secondGlobalPos);

        res[2] = TransformExtensions.InverseTransformPointUnscaled(planePositionEnd, planeDirectionEnd, sphereFinish);

        return res;
    }

    Vector3[] LocalizePathNormalized(
        Vector3 planePositionStart,
        Vector3 planeDirectionStart,
        Vector3 planePositionEnd,
        Vector3 planeDirectionEnd,
        Vector3 sphereStart,
        Vector3 sphereFinish,
        float time)
    {
        Vector3[] res = new Vector3[3];

        Vector3 localPos1 = TransformExtensions.InverseTransformPointUnscaled(planePositionStart, planeDirectionStart, sphereStart);
        res[0] = localPos1.normalized;

        Vector3 planeMidPos = Vector3.Lerp(planePositionStart, planePositionEnd, 0.5f);
        Vector3 planeMidDir = Vector3.Lerp(planeDirectionStart, planeDirectionEnd, 0.5f);
        Vector3 secondGlobalPos = Vector3.Lerp(sphereStart, sphereFinish, 0.5f);

        Vector3 localPos2 = TransformExtensions.InverseTransformPointUnscaled(planeMidPos, planeMidDir, secondGlobalPos);
        res[1] = localPos2.normalized;

        Vector3 localPos3 = TransformExtensions.InverseTransformPointUnscaled(planePositionEnd, planeDirectionEnd, sphereFinish);
        res[2] = localPos3.normalized;

        return res;
    }

    float[] Regression(Vector2[] points)
    {
        float[] res = new float[3];

        LstSquQuadRegr solvr = new LstSquQuadRegr();

        solvr.AddPoints(points[0].x, points[0].y);
        solvr.AddPoints(points[1].x, points[1].y);
        solvr.AddPoints(points[2].x, points[2].y);

        res[0] = (float)solvr.aTerm();
        res[1] = (float)solvr.bTerm();
        res[2] = (float)solvr.cTerm();

        return res;
    }

    void DebugPlot_ZY(float a, float b, float c, float start_x, float endx, float padding, int iterations, bool y_is_x)
    {

        if (endx < start_x)
        {
            float tmp = start_x;
            start_x = endx;
            endx = tmp;
        }


        start_x -= (padding);
        endx += (padding);

        float cur_x = start_x;
        float step = Mathf.Abs(endx - start_x) / iterations;

        

        for (int i = 0; i <= iterations; i++)
        {
            float y1 = (float)QuadraticSolveY(a, b, c, cur_x);
            float y2 = (float)QuadraticSolveY(a, b, c, cur_x + step);

            if (y_is_x)
            {
                Debug.DrawLine(new Vector3(0, (float)cur_x, y1), new Vector3(0, (float)(cur_x + step), y2), Color.red, 5000);
            }
            else
            {
                Debug.DrawLine(new Vector3(0, y1, (float)cur_x), new Vector3(0, y2, (float)(cur_x + step)), Color.red, 5000);
            }

            cur_x += step;
        }
    }

    void DebugPlot_ZX(float a, float b, float c, float start_x, float endx, float padding, int iterations, bool y_is_x)
    {

        if (endx < start_x)
        {
            float tmp = start_x;
            start_x = endx;
            endx = tmp;
        }

        start_x -= (padding);
        endx += (padding);

        Debug.LogFormat("Start at {0}", start_x);
        Debug.LogFormat("End at {0}", endx);

        float cur_x = start_x;
        float step = Mathf.Abs(endx - start_x) / iterations;



        for (int i = 0; i <= iterations; i++)
        {
            float y1 = (float)QuadraticSolveY(a, b, c, cur_x);
            float y2 = (float)QuadraticSolveY(a, b, c, cur_x + step);

            if (y_is_x)
            {
                Debug.DrawLine(new Vector3((float)cur_x, 0, y1), new Vector3((float)(cur_x + step), 0, y2), Color.red, 5000);
            }
            else
            {
                Debug.DrawLine(new Vector3(y1, 0, (float)cur_x), new Vector3(y2, 0, (float)(cur_x + step)), Color.red, 5000);
            }

            cur_x += step;
        }
    }

    float GetArcLength(float x1, float x2, float[] coef)
    {
        // make sure they are in correct order
        float start = Mathf.Min(x1, x2);
        float finish = Mathf.Max(x1, x2);

        float arcLenInteg = QuadraticArcLength2(coef[0], coef[1], coef[2], start, finish, false);
        //float arcLenEst = QuadraticArcLengthEstimate(coef[0], coef[1], coef[2], start, finish, 1000000);

        //Debug.Log("Arc Length: " + arcLenInteg);
        //Debug.Log("Leng Est: " + arcLenEst);

        return arcLenInteg;
    }

    double QuadraticSolveY(double a, double b, double c, double x)
    {
        return (float)((a * System.Math.Pow(x, 2)) + (b * x) + c);
    }

    float QuadraticArcLength(float a, float b, float c, float t)
    {
        float scale_factor = 1.0f / t;

        Debug.LogFormat("Scale Factor: {0}", scale_factor);

        float x1 = 0;
        float y1 = (float)QuadraticSolveY(a, b, c, x1);
        Debug.LogFormat("xy1: {0}, {1}", x1, y1);

        float x2 = t * 0.5f;
        float y2 = (float)QuadraticSolveY(a, b, c, x2);
        Debug.LogFormat("xy2: {0}, {1}", x2, y2);

        float x3 = t;
        float y3 = (float)QuadraticSolveY(a, b, c, x3);
        Debug.LogFormat("xy3: {0}, {1}", x3, y3);

        LstSquQuadRegr solvr = new LstSquQuadRegr();
        solvr.AddPoints(0, y1 * scale_factor);
        solvr.AddPoints(0.5, y2 * scale_factor);
        solvr.AddPoints(1, y3 * scale_factor);
        //solvr.PreCalculate();

        float na = (float)solvr.aTerm();
        float nb = (float)solvr.bTerm();
        float nc = (float)solvr.cTerm();
        Debug.LogFormat("Normalized equation: {0}x^2 + {1}x + {2}", na, nb, nc);

        float n_len = QuadraticArcLength(na, nb, nc);
        Debug.LogFormat("Normalized Value: {0}", n_len);

        return (float)(n_len / (float)scale_factor);
    }

    float QuadraticArcLength(double a, double b, double c)
    {
        double i = 0;
        double j = 1;

        double aj = a * j;
        double ai = a * i;
        double _2aj = 2 * aj;
        double _2ai = 2 * ai;

        double sqrt_1_2aj_b_2 = System.Math.Sqrt(1 + System.Math.Pow(_2aj + b, 2));
        double sqrt_1_2ai_b_2 = System.Math.Sqrt(1 + System.Math.Pow(_2ai + b, 2));

        double s1 = _2aj * sqrt_1_2aj_b_2;
        double s2 = _2ai * sqrt_1_2ai_b_2;
        double s3 = b * sqrt_1_2aj_b_2;
        double s4 = b * sqrt_1_2ai_b_2;
        double s5 = System.Math.Log(System.Math.Abs(_2aj + b + sqrt_1_2aj_b_2));
        double s6 = System.Math.Log(System.Math.Abs(_2ai + b + sqrt_1_2ai_b_2));

        return (float)((s1 - s2 + s3 - s4 + s5 - s6) / (4 * a));

    }

    float QuadraticArcLength2(float a, float b, float c, float i, float j, bool doublePercision)
    {
        if (doublePercision)
        {
            double aj = a * j;
            double ai = a * i;
            double _2aj = 2 * aj;
            double _2ai = 2 * ai;

            double sqrt_1_2aj_b_2 = System.Math.Sqrt(1 + System.Math.Pow(_2aj + b, 2));
            double sqrt_1_2ai_b_2 = System.Math.Sqrt(1 + System.Math.Pow(_2ai + b, 2));

            double s1 = _2aj * sqrt_1_2aj_b_2;
            double s2 = _2ai * sqrt_1_2ai_b_2;
            double s3 = b * sqrt_1_2aj_b_2;
            double s4 = b * sqrt_1_2ai_b_2;
            double s5 = System.Math.Log(System.Math.Abs(_2aj + b + sqrt_1_2aj_b_2));
            double s6 = System.Math.Log(System.Math.Abs(_2ai + b + sqrt_1_2ai_b_2));

            return (float)((s1 - s2 + s3 - s4 + s5 - s6) / (4 * a));
        }
        else
        {
            float aj = a * j;
            float ai = a * i;
            float _2aj = 2 * aj;
            float _2ai = 2 * ai;

            float sqrt_1_2aj_b_2 = Mathf.Sqrt(1 + Mathf.Pow(_2aj + b, 2));
            float sqrt_1_2ai_b_2 = Mathf.Sqrt(1 + Mathf.Pow(_2ai + b, 2));

            float s1 = _2aj * sqrt_1_2aj_b_2;
            float s2 = _2ai * sqrt_1_2ai_b_2;
            float s3 = b * sqrt_1_2aj_b_2;
            float s4 = b * sqrt_1_2ai_b_2;
            float s5 = Mathf.Log(Mathf.Abs(_2aj + b + sqrt_1_2aj_b_2));
            float s6 = Mathf.Log(Mathf.Abs(_2ai + b + sqrt_1_2ai_b_2));

            return (float)((s1 - s2 + s3 - s4 + s5 - s6) / (4 * a));
        }

            

    }

    float[] QuadraticFormula(float a, float b, float c)
    {
        float sqrt = Mathf.Sqrt(Mathf.Pow(b, 2) - 4 * a * c);
        float _2a = 2 * a;

        float ans1 = (-b - sqrt) / _2a;
        float ans2 = (-b + sqrt) / _2a;

        return new[] {ans1, ans2 };
    }

    /*float QuadraticArcLength(double a, double b, double c, double t)
    {
        double _2at = 2 * a * t;

        double sqrt_1_b_2_at_p2 = System.Math.Sqrt(1 + System.Math.Pow(b + _2at, 2));

        double sqrt_1_b_p2 = System.Math.Sqrt(1 + System.Math.Pow(b, 2));

        double s1 = _2at * sqrt_1_b_2_at_p2;
        double s2 = b * sqrt_1_b_2_at_p2;
        double s3 = b * sqrt_1_b_p2;
        double s4 = System.Math.Log(System.Math.Abs(b + _2at + sqrt_1_b_2_at_p2));
        double s5 = System.Math.Log(System.Math.Abs(sqrt_1_b_p2));

        return (float)((s1 + s2 - s3 + s4 - s5) / (4 * a));

    }*/

    float QuadraticAricLengthNormalized(double a, double b, double c)
    {
        double _2a = 2 * a;

        double sqrt_1_b_2_a_p2 = System.Math.Sqrt(1 + System.Math.Pow(b + _2a, 2));
        double sqrt_1_b_p2 = System.Math.Sqrt(1 + System.Math.Pow(b, 2));

        double s1 = _2a * sqrt_1_b_2_a_p2;
        double s2 = b * sqrt_1_b_2_a_p2;
        double s3 = b * sqrt_1_b_p2;
        double s4 = System.Math.Log(System.Math.Abs(b + _2a + sqrt_1_b_2_a_p2));
        double s5 = System.Math.Log(System.Math.Abs(sqrt_1_b_p2));

        return (float)((s1 + s2 - s3 + s4 - s5) / (4 * a));
    }

    float QuadraticArcLengthEstimate(float a, float b, float c, float i, float j, int iterations)
    {
        double sum_res = 0;

        double x_step = System.Math.Abs(j - i)/ iterations;
        double x_step_half = x_step / 2.0f;
        double x = i;

        double last_y = QuadraticSolveY(a, b, c, x + x_step_half);
        for (int it = 0; it <= iterations; it++)
        {
            double x1 = x + x_step_half;
            double x2 = x1 + x_step;

            double y1 = last_y;//QuadraticSolveY(a, b, c, x1);//pow(a * x1, 2) + (b * x1) + c;
            double y2 = QuadraticSolveY(a, b, c, x2);//pow(a * x2, 2) + (b * x2) + c;
            last_y = y2;

            sum_res += System.Math.Sqrt(System.Math.Pow(x2 - x1, 2) + System.Math.Pow(y2 - y1, 2));


            x += x_step;
        }

        return (float)sum_res;
    }

    
}
