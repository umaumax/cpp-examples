struct f2 {
  float a;
  float b;

  // NOTE: []内が定数の場合にはfieldに直接アクセスするコードとなる
  float operator[](int i) const { return i == 0 ? a : b; }
  float& operator[](int i) { return i == 0 ? a : b; }
  // NOTE: 0と1のみしか指定されないことを前提とした裏技
  // この構造体が実質float[2]と同じになる(indexへのアクセスのasmも同じだった)
  //   float& operator[](int i) { return ((float*)(&a))[i]; }
};

// same code
float f_array(float fs[2]) { return fs[0] + fs[1]; }
float f_struct_alias(f2& fs) { return fs.a + fs.b; }
float f_struct_pointer(f2* fs) { return fs->a + fs->b; }

// same code
float f_array_pointer(float* fs[2]) { return (*fs)[0] + (*fs)[1]; }

// same code
float f_struct_copy_field(f2 fs) { return fs.a + fs.b; }
float f_struct_copy_index(f2 fs) { return fs[0] + fs[1]; }
