/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

const EPSILON = 0.000001;

class Vector3 {
  constructor(x, y, z) {
    this.x = x || 0.0;
    this.y = y || 0.0;
    this.z = z || 0.0;
  }

  magnitude(v) {
    //Only get the magnitude of this vector
    if (v === undefined)
      return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);

    //Get magnitude based on another vector
    var x = v.x - this.x,
      y = v.y - this.y,
      z = v.y - this.z;

    return Math.sqrt(x * x + y * y + z * z);
  }

  normalize() {
    var mag = this.magnitude();
    this.x /= mag;
    this.y /= mag;
    this.z /= mag;
    return this;
  }

  getNormalized() {
    var mag = this.magnitude();
    return new Float32Array([this.x / mag, this.y / mag, this.z / mag]);
  }

  set(x, y, z) {
    this.x = x;
    this.y = y;
    this.z = z;
    return this;
  }

  multiScalar(v) {
    this.x *= v;
    this.y *= v;
    this.z *= v;
    return this;
  }

  getArray() {
    return [this.x, this.y, this.z];
  }
  getFloatArray() {
    return new Float32Array([this.x, this.y, this.z]);
  }
  clone() {
    return new Vector3(this.x, this.y, this.z);
  }

  getRight() {
    return this.softRotateY((90 * Math.PI) / 180);
  }

  getLeft() {
    return this.softRotateY((-90 * Math.PI) / 180);
  }

  softRotateY(rad) {
    let temp = new Float32Array([0, 0, 0]);
    Vector3.rotateY(
      temp,
      this.getNormalized(),
      new Float32Array([0, 0, 0]),
      rad
    );
    return temp;
  }

  rotateY(rad) {
    let temp = new Float32Array([0, 0, 0]);
    Vector3.rotateY(
      temp,
      this.getFloatArray(),
      new Float32Array([0, 0, 0]),
      rad
    );
    this.set(temp[0], temp[1], temp[2]);
    return this;
  }
  rotateX(rad) {
    let temp = new Float32Array([0, 0, 0]);
    Vector3.rotateX(
      temp,
      this.getFloatArray(),
      new Float32Array([0, 0, 0]),
      rad
    );
    this.set(temp[0], temp[1], temp[2]);
    return this;
  }
  rotateZ(rad) {
    let temp = new Float32Array([0, 0, 0]);
    Vector3.rotateZ(
      temp,
      this.getFloatArray(),
      new Float32Array([0, 0, 0]),
      rad
    );
    this.set(temp[0], temp[1], temp[2]);
    return this;
  }

  static rotateX(out, a, b, rad) {
    let p = [],
      r = [];
    //Translate point to the origin
    p[0] = a[0] - b[0];
    p[1] = a[1] - b[1];
    p[2] = a[2] - b[2];
    //perform rotation
    r[0] = p[0];
    r[1] = p[1] * Math.cos(rad) - p[2] * Math.sin(rad);
    r[2] = p[1] * Math.sin(rad) + p[2] * Math.cos(rad);
    //translate to correct position
    out[0] = r[0] + b[0];
    out[1] = r[1] + b[1];
    out[2] = r[2] + b[2];
    return out;
  }

  static rotateY(out, a, b, rad) {
    let p = [],
      r = [];
    //Translate point to the origin
    p[0] = a[0] - b[0];
    p[1] = a[1] - b[1];
    p[2] = a[2] - b[2];
    //perform rotation
    r[0] = p[2] * Math.sin(rad) + p[0] * Math.cos(rad);
    r[1] = p[1];
    r[2] = p[2] * Math.cos(rad) - p[0] * Math.sin(rad);
    //translate to correct position
    out[0] = r[0] + b[0];
    out[1] = r[1] + b[1];
    out[2] = r[2] + b[2];
    return out;
  }

  static rotateZ(out, a, b, rad) {
    let p = [],
      r = [];
    //Translate point to the origin
    p[0] = a[0] - b[0];
    p[1] = a[1] - b[1];
    p[2] = a[2] - b[2];
    //perform rotation
    r[0] = p[0] * Math.cos(rad) - p[1] * Math.sin(rad);
    r[1] = p[0] * Math.sin(rad) + p[1] * Math.cos(rad);
    r[2] = p[2];
    //translate to correct position
    out[0] = r[0] + b[0];
    out[1] = r[1] + b[1];
    out[2] = r[2] + b[2];
    return out;
  }

  static dot(a, b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
  }
}

class Matrix4 {
  constructor() {
    this.raw = Matrix4.identity();
  }

  vtranslate(v) {
    Matrix4.translate(this.raw, v.x, v.y, v.z);
    return this;
  }

  rotateY(rad) {
    Matrix4.rotateY(this.raw, rad);
    return this;
  }
  rotateX(rad) {
    Matrix4.rotateX(this.raw, rad);
    return this;
  }
  rotateZ(rad) {
    Matrix4.rotateZ(this.raw, rad);
    return this;
  }

  vscale(vec3) {
    Matrix4.scale(this.raw, vec3.x, vec3.y, vec3.z);
    return this;
  }

  //reset data back to identity.
  reset() {
    for (var i = 0; i < this.raw.length; i++) this.raw[i] = i % 5 == 0 ? 1 : 0; //only positions 0,5,10,15 need to be 1 else 0.
    return this;
  }

  //Static Methods
  static identity() {
    var a = new Float32Array(16);
    a[0] = a[5] = a[10] = a[15] = 1;
    return a;
  }

  //from glMatrix
  static perspective(out, fovy, aspect, near, far) {
    let f = 1.0 / Math.tan(fovy / 2),
      nf = 1 / (near - far);
    out[0] = f / aspect;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = f;
    out[6] = 0;
    out[7] = 0;
    out[8] = 0;
    out[9] = 0;
    out[10] = (far + near) * nf;
    out[11] = -1;
    out[12] = 0;
    out[13] = 0;
    out[14] = 2 * far * near * nf;
    out[15] = 0;
  }

  static lookAt(out, eye, center, up) {
    let x0, x1, x2, y0, y1, y2, z0, z1, z2, len;
    let eyex = eye[0];
    let eyey = eye[1];
    let eyez = eye[2];
    let upx = up[0];
    let upy = up[1];
    let upz = up[2];
    let centerx = center[0];
    let centery = center[1];
    let centerz = center[2];
    if (
      Math.abs(eyex - centerx) < EPSILON &&
      Math.abs(eyey - centery) < EPSILON &&
      Math.abs(eyez - centerz) < EPSILON
    ) {
      return identity(out);
    }
    z0 = eyex - centerx;
    z1 = eyey - centery;
    z2 = eyez - centerz;
    len = 1 / Math.hypot(z0, z1, z2);
    z0 *= len;
    z1 *= len;
    z2 *= len;
    x0 = upy * z2 - upz * z1;
    x1 = upz * z0 - upx * z2;
    x2 = upx * z1 - upy * z0;
    len = Math.hypot(x0, x1, x2);
    if (!len) {
      x0 = 0;
      x1 = 0;
      x2 = 0;
    } else {
      len = 1 / len;
      x0 *= len;
      x1 *= len;
      x2 *= len;
    }
    y0 = z1 * x2 - z2 * x1;
    y1 = z2 * x0 - z0 * x2;
    y2 = z0 * x1 - z1 * x0;
    len = Math.hypot(y0, y1, y2);
    if (!len) {
      y0 = 0;
      y1 = 0;
      y2 = 0;
    } else {
      len = 1 / len;
      y0 *= len;
      y1 *= len;
      y2 *= len;
    }
    out.raw[0] = x0;
    out.raw[1] = y0;
    out.raw[2] = z0;
    out.raw[3] = 0;
    out.raw[4] = x1;
    out.raw[5] = y1;
    out.raw[6] = z1;
    out.raw[7] = 0;
    out.raw[8] = x2;
    out.raw[9] = y2;
    out.raw[10] = z2;
    out.raw[11] = 0;
    out.raw[12] = -(x0 * eyex + x1 * eyey + x2 * eyez);
    out.raw[13] = -(y0 * eyex + y1 * eyey + y2 * eyez);
    out.raw[14] = -(z0 * eyex + z1 * eyey + z2 * eyez);
    out.raw[15] = 1;
    return out;
  }

  //Calculates a 3x3 normal matrix (transpose inverse) from the 4x4 matrix
  static normalMat3(out, a) {
    var a00 = a[0],
      a01 = a[1],
      a02 = a[2],
      a03 = a[3],
      a10 = a[4],
      a11 = a[5],
      a12 = a[6],
      a13 = a[7],
      a20 = a[8],
      a21 = a[9],
      a22 = a[10],
      a23 = a[11],
      a30 = a[12],
      a31 = a[13],
      a32 = a[14],
      a33 = a[15],
      b00 = a00 * a11 - a01 * a10,
      b01 = a00 * a12 - a02 * a10,
      b02 = a00 * a13 - a03 * a10,
      b03 = a01 * a12 - a02 * a11,
      b04 = a01 * a13 - a03 * a11,
      b05 = a02 * a13 - a03 * a12,
      b06 = a20 * a31 - a21 * a30,
      b07 = a20 * a32 - a22 * a30,
      b08 = a20 * a33 - a23 * a30,
      b09 = a21 * a32 - a22 * a31,
      b10 = a21 * a33 - a23 * a31,
      b11 = a22 * a33 - a23 * a32,
      // Calculate the determinant
      det =
        b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

    if (!det) return null;

    det = 1.0 / det;

    out[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
    out[1] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
    out[2] = (a10 * b10 - a11 * b08 + a13 * b06) * det;

    out[3] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
    out[4] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
    out[5] = (a01 * b08 - a00 * b10 - a03 * b06) * det;

    out[6] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
    out[7] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
    out[8] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
    return out;
  }

  static scale(out, x, y, z) {
    out[0] *= x;
    out[1] *= x;
    out[2] *= x;
    out[3] *= x;
    out[4] *= y;
    out[5] *= y;
    out[6] *= y;
    out[7] *= y;
    out[8] *= z;
    out[9] *= z;
    out[10] *= z;
    out[11] *= z;
    return out;
  }

  static rotateY(out, rad) {
    var s = Math.sin(rad),
      c = Math.cos(rad),
      a00 = out[0],
      a01 = out[1],
      a02 = out[2],
      a03 = out[3],
      a20 = out[8],
      a21 = out[9],
      a22 = out[10],
      a23 = out[11];

    // Perform axis-specific matrix multiplication
    out[0] = a00 * c - a20 * s;
    out[1] = a01 * c - a21 * s;
    out[2] = a02 * c - a22 * s;
    out[3] = a03 * c - a23 * s;
    out[8] = a00 * s + a20 * c;
    out[9] = a01 * s + a21 * c;
    out[10] = a02 * s + a22 * c;
    out[11] = a03 * s + a23 * c;
    return out;
  }

  static rotateX(out, rad) {
    var s = Math.sin(rad),
      c = Math.cos(rad),
      a10 = out[4],
      a11 = out[5],
      a12 = out[6],
      a13 = out[7],
      a20 = out[8],
      a21 = out[9],
      a22 = out[10],
      a23 = out[11];

    // Perform axis-specific matrix multiplication
    out[4] = a10 * c + a20 * s;
    out[5] = a11 * c + a21 * s;
    out[6] = a12 * c + a22 * s;
    out[7] = a13 * c + a23 * s;
    out[8] = a20 * c - a10 * s;
    out[9] = a21 * c - a11 * s;
    out[10] = a22 * c - a12 * s;
    out[11] = a23 * c - a13 * s;
    return out;
  }

  static rotateZ(out, rad) {
    var s = Math.sin(rad),
      c = Math.cos(rad),
      a00 = out[0],
      a01 = out[1],
      a02 = out[2],
      a03 = out[3],
      a10 = out[4],
      a11 = out[5],
      a12 = out[6],
      a13 = out[7];

    // Perform axis-specific matrix multiplication
    out[0] = a00 * c + a10 * s;
    out[1] = a01 * c + a11 * s;
    out[2] = a02 * c + a12 * s;
    out[3] = a03 * c + a13 * s;
    out[4] = a10 * c - a00 * s;
    out[5] = a11 * c - a01 * s;
    out[6] = a12 * c - a02 * s;
    out[7] = a13 * c - a03 * s;
    return out;
  }

  static invert(out, mat) {
    if (mat === undefined) mat = out; //If input isn't sent, then output is also input

    var a00 = mat[0],
      a01 = mat[1],
      a02 = mat[2],
      a03 = mat[3],
      a10 = mat[4],
      a11 = mat[5],
      a12 = mat[6],
      a13 = mat[7],
      a20 = mat[8],
      a21 = mat[9],
      a22 = mat[10],
      a23 = mat[11],
      a30 = mat[12],
      a31 = mat[13],
      a32 = mat[14],
      a33 = mat[15],
      b00 = a00 * a11 - a01 * a10,
      b01 = a00 * a12 - a02 * a10,
      b02 = a00 * a13 - a03 * a10,
      b03 = a01 * a12 - a02 * a11,
      b04 = a01 * a13 - a03 * a11,
      b05 = a02 * a13 - a03 * a12,
      b06 = a20 * a31 - a21 * a30,
      b07 = a20 * a32 - a22 * a30,
      b08 = a20 * a33 - a23 * a30,
      b09 = a21 * a32 - a22 * a31,
      b10 = a21 * a33 - a23 * a31,
      b11 = a22 * a33 - a23 * a32,
      // Calculate the determinant
      det =
        b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

    if (!det) return false;
    det = 1.0 / det;

    out[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
    out[1] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
    out[2] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
    out[3] = (a22 * b04 - a21 * b05 - a23 * b03) * det;
    out[4] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
    out[5] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
    out[6] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
    out[7] = (a20 * b05 - a22 * b02 + a23 * b01) * det;
    out[8] = (a10 * b10 - a11 * b08 + a13 * b06) * det;
    out[9] = (a01 * b08 - a00 * b10 - a03 * b06) * det;
    out[10] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
    out[11] = (a21 * b02 - a20 * b04 - a23 * b00) * det;
    out[12] = (a11 * b07 - a10 * b09 - a12 * b06) * det;
    out[13] = (a00 * b09 - a01 * b07 + a02 * b06) * det;
    out[14] = (a31 * b01 - a30 * b03 - a32 * b00) * det;
    out[15] = (a20 * b03 - a21 * b01 + a22 * b00) * det;

    return true;
  }

  static translate(out, x, y, z) {
    out[12] = out[0] * x + out[4] * y + out[8] * z + out[12];
    out[13] = out[1] * x + out[5] * y + out[9] * z + out[13];
    out[14] = out[2] * x + out[6] * y + out[10] * z + out[14];
    out[15] = out[3] * x + out[7] * y + out[11] * z + out[15];
  }
}

const ATTR_POSITION_NAME = "a_position";
const ATTR_POSITION_LOC = 0;
const ATTR_NORMAL_NAME = "a_norm";
const ATTR_NORMAL_LOC = 1;
const ATTR_UV_NAME = "a_uv";
const ATTR_UV_LOC = 2;
