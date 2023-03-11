using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class TransformExtensions
{
    public static Vector3 TransformPointUnscaled(Transform transform, Vector3 position)
    {
        var localToWorldMatrix = Matrix4x4.TRS(transform.position, transform.rotation, Vector3.one);
        return localToWorldMatrix.MultiplyPoint3x4(position);
    }

    public static Vector3 InverseTransformPointUnscaled(Transform transform, Vector3 position)
    {
        var worldToLocalMatrix = Matrix4x4.TRS(transform.position, transform.rotation, Vector3.one).inverse;
        return worldToLocalMatrix.MultiplyPoint3x4(position);
    }

    public static Vector3 TransformPointUnscaled(Vector3 object_position, Vector3 object_direction, Vector3 position)
    {
        Quaternion.LookRotation(object_direction, Vector3.up);
        var localToWorldMatrix = Matrix4x4.TRS(object_position, Quaternion.LookRotation(object_direction, Vector3.up), Vector3.one);
        return localToWorldMatrix.MultiplyPoint3x4(position);
    }

    public static Vector3 InverseTransformPointUnscaled(Vector3 object_position, Vector3 object_direction, Vector3 position)
    {
        var worldToLocalMatrix = Matrix4x4.TRS(object_position, Quaternion.LookRotation(object_direction, Vector3.up), Vector3.one).inverse;
        return worldToLocalMatrix.MultiplyPoint3x4(position);
    }
}
