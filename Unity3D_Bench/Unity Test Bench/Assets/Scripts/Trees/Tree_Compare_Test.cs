using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static CL_Tree_Testing;

public class Tree_Compare_Test : MonoBehaviour
{
    public int numEntries;
    public CL_Tree_Testing CL_Tree;
    public Tree_Tests CSharp_Tree;

    BoundingBox[] cshare_bounds;
    BoundingBox_CL[] cl_bounds;

    // Start is called before the first frame update
    void Start()
    {
        //return;

        Debug.LogFormat("Tree size in Local Memory: {0}", CL_Tree.GetLocalMemorySizeEstimate(numEntries));

        CSharp_Tree.Init(numEntries);
        CL_Tree.Init(numEntries);

        //return;

        
        CreateBoundingBoxes();

        BenchTest();

        CompareResults();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void CreateBoundingBoxes()
    {
        cshare_bounds = new BoundingBox[numEntries];
        cl_bounds = new BoundingBox_CL[numEntries];

        for (int i = 0; i < numEntries; i++)
        {
            Vector3 pos = new Vector3();
            pos.x = Random.Range(-100f, 100f);
            pos.y = Random.Range(-100f, 100f);
            pos.z = Random.Range(-100f, 100f);

            Vector3 scale = Vector3.one;

            object[] bb = GetCubeBounds(pos, scale);

            cshare_bounds[i] = (BoundingBox)bb[0];
            cl_bounds[i] = (BoundingBox_CL)bb[1];
        }
    }

    void BenchTest()
    {
        System.Diagnostics.Stopwatch watch  = new System.Diagnostics.Stopwatch();

        watch.Start();
        CSharp_Tree.DoTreeInsertTest2(cshare_bounds);
        watch.Stop();
        Debug.LogFormat("CSharp Tree Time: {0}", watch.Elapsed.TotalSeconds);

        watch.Restart();
        double kernel_time = CL_Tree.AddBoxes(cl_bounds);
        watch.Stop();
        Debug.LogFormat("CL Tree Time: {0} (kernel: {1})", watch.Elapsed.TotalSeconds, kernel_time);
    }

    void CompareResults()
    {
        CL_Tree.ExtractInfo(numEntries);

        // Compare Nodes
        if (CSharp_Tree.tree.NodeCount != CL_Tree.NodeCount)
        {
            Debug.LogErrorFormat("Node count mis-match!");
            return;
        }
        for (int i = 0; i < numEntries - 1; i++)
        {
            if (!CSharp_Tree.tree.Nodes[i].Equals(CL_Tree.Nodes[i]))
            {
                Debug.LogErrorFormat("Node mis-match: {0}", i);
                return;
            }
        }

        // Compare Metanodes
        for (int i = 0; i < numEntries - 1; i++)
        {
            if (!CSharp_Tree.tree.Metanodes[i].Equals(CL_Tree.Metanodes[i]))
            {
                Debug.LogErrorFormat("Metanode mis-match: {0}", i);
                return;
            }
        }

        // Compare Leaves
        if (CSharp_Tree.tree.LeafCount != CL_Tree.LeafCount)
        {
            Debug.LogErrorFormat("Leaf count mis-match!");
            return;
        }
        for (int i = 0; i < numEntries; i++)
        {
            if (!CSharp_Tree.tree.Leaves[i].Equals(CL_Tree.Leaves[i]))
            {
                Debug.LogErrorFormat("Leaf mis-match: {0}", i);
                return;
            }
        }

        Debug.Log("CL and CSharp Tree match!");
    }

    object[] GetCubeBounds(Vector3 position, Vector3 scale)
    {
        Vector3 pos = position;

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
        return new object[]{new BoundingBox(min, max), new BoundingBox_CL(min, max)};
    }
}
