using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ComputeTree_Tests : MonoBehaviour
{
    public int DATA_SIZE = 32;
    public ComputeTree tree;

    public bool useKernel;

    public int BoundsToProcess;
    public Transform[] Cubes;

    // Start is called before the first frame update
    void Start()
    {
        //DoComputeTest();

        //DoTreeInsertTest1();
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void Init(int num)
    {
        tree = new ComputeTree(num, useKernel);
    }

    BoundingBox_CL GetCubeBounds(Transform obj)
    {
        Vector3 pos = obj.position;
        Vector3 scale = obj.lossyScale;

        Vector3 min = new Vector3(
            (-0.5f * scale.x) + pos.x,
            (-0.5f * scale.y) + pos.y,
            (-0.5f * scale.z) + pos.z
        );
        Vector3 max = new Vector3(
            (0.5f * scale.x) + pos.x,
            (0.5f * scale.y) + pos.y,
            (0.5f * scale.z) + pos.z
        );
        return new BoundingBox_CL(min, max);
    }

    public void DoTreeInsertTest1()
    {
        tree = new ComputeTree(10, useKernel);

        BoundingBox_CL[] boundsAdd = new BoundingBox_CL[10];
        for (int i = 0; i < Mathf.Min(BoundsToProcess, Cubes.Length); i++)
        {
            boundsAdd[i] = GetCubeBounds(Cubes[i]);
            //int leafInd = tree.Add(bounds);
            //Debug.LogFormat("New Leaf: {0}", leafInd);
        }

        tree.Add(boundsAdd, true);
    }

    public double DoTreeInsertTest2(BoundingBox_CL[] bounds)
    {
        System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();

        watch.Start();
        tree.Add(bounds, false);
        watch.Stop();

        tree.FlushAll(true);

        return watch.Elapsed.TotalSeconds;

    }
}
