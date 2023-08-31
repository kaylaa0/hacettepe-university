/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

class Vec3 extends Float32Array {
  constructor(ini) {
    super(3);
    if (ini instanceof Vec3) {
      this[0] = ini[0];
      this[1] = ini[1];
      this[2] = ini[2];
    } else {
      this[0] = this[1] = this[2] = ini || 0;
    }
    this.isModified = true;
  }

  set(x, y, z) {
    this[0] = x;
    this[1] = y;
    this[2] = z;
    this.isModified = true;
    return this;
  }

  get x() {
    return this[0];
  }
  set x(val) {
    this[0] = val;
    this.isModified = true;
  }
  get y() {
    return this[1];
  }
  set y(val) {
    this[1] = val;
    this.isModified = true;
  }
  get z() {
    return this[2];
  }
  set z(val) {
    this[2] = val;
    this.isModified = true;
  }

  multi(v) {
    this[0] *= v;
    this[1] *= v;
    this[2] *= v;
    this.isModified = true;
    return this;
  }
}

function vec4(x1, x2, x3, x4) {
  this.x = x1;
  this.y = x2;
  this.z = x3;
  this.r = x1;
  this.g = x2;
  this.b = x3;
  this.a = x4;
  this.alpha = x4;
  this.elements = function () {
    return [this.x, this.y, this.z, this.alpha];
  };
}

const point4 = vec4;
const color4 = vec4;

var m4 = {
  projection: function (width, height, depth) {
    return [
      1 / width,
      0,
      0,
      0,
      0,
      1 / height,
      0,
      0,
      0,
      0,
      1 / depth,
      0,
      0,
      0,
      0,
      1,
    ];
  },
};

var matrix = m4.projection(2.5, 2.5, 2.5);
