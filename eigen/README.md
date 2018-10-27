# eigen example

## quaternionを回転行列に変換する
```
Eigen::Quaternionf q;
q.x() = 0.0;
q.y() = 0.0;
q.z() = 0.0;
q.w() = 1.0;
Eigen::Matrix3f m = q.normalized().toRotationMatrix();
```

## 位置をquaternion分回転させる
(行列の演算と同様の順序と覚えれば良い)
```
Eigen::Quaternionf q;
Eigen::Vector3f v(0.0, 0.0, 0.0);
v = q * v;
```

## 座標系2の回転を座標系1に変換する
座標系1を座標系2に合わせてから，座標系2上で回転させてから，座標系1に戻す
```
Eigen::Quaternionf q2;
Eigen::Matrix3f m12; // 座標系を座標系'の回転行列
m12 << 0.0, -1.0,  0.0,
       0.0,  0.0, -1.0,
       1.0,  0.0,  0.0;
Eigen::Matrix3f m1 = m12 * q.normalized().toRotationMatrix() * m12.inverse();
```

## FYI
[Quaternions \- Visualisation]( https://quaternions.online/ )
