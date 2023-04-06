using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices.WindowsRuntime;
using UnityEngine;

public class Tree_Tests : MonoBehaviour
{
    public TestTree tree;

    public Transform[] Cubes;

    // Start is called before the first frame update
    void Start()
    {
        for (int i = 0; i < 100; i++)
        {
            Debug.LogFormat("GetContainingPowerOf2: {0} = {1}", i, TestTree.GetContainingPowerOf2(i));
        }

        //DoTreeInsertTest1();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    BoundingBox GetCubeBounds(Transform obj)
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
        return new BoundingBox(min, max);
    }

    void DoTreeInsertTest1()
    {
        tree = new TestTree(10);

        for (int i = 0; i < Cubes.Length; i++)
        {
            BoundingBox bounds = GetCubeBounds(Cubes[i]);
            int leafInd = tree.Add(bounds);
            Debug.LogFormat("New Leaf: {0}", leafInd);
        }

    }
}
