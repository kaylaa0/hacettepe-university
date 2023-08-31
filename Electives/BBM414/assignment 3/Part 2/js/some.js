/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

class Bezier {
  static generate(out, res, p0, p1, p2, p3) {
    // Inputs, out = the points/vertex data array, res = Bezier function resolution higher the resolution more lines to create curve, p0 and p3 are points to create curve in between, p1 and p2 are the weight points.
    var inc = 1 / res, // Increment depends on resolution, higher resolution means lower increment, means more lines for curve.
      pos = new Vec3(); // Temp vector to hold our current position along curve.
    for (var i = 0; i < res; i++) {
      Bezier.getPoint(p0, p1, p2, p3, i * inc, pos); // Gets current calculated point along Bezier Curve.
      out.push(new point4(pos.x, pos.y, pos.z, 1)); // Puts it in output array.
    }
    Bezier.getPoint(p0, p1, p2, p3, 1, pos); // Lastly puts the last point in the array too. Last point is also the p3.
    out.push(new point4(pos.x, pos.y, pos.z, 1));
  }

  static getPoint(p0, p1, p2, p3, t, out) {
    if (t > 1) t = 1;
    else if (t < 0) t = 0;

    var i = 1 - t;
    // Specified Bezier Function.
    out = out || new Vec3();
    out[0] =
      i * i * i * p0.x +
      3 * i * i * t * p1.x +
      3 * i * t * t * p2.x +
      t * t * t * p3.x;
    out[1] =
      i * i * i * p0.y +
      3 * i * i * t * p1.y +
      3 * i * t * t * p2.y +
      t * t * t * p3.y;
    out[2] =
      i * i * i * p0.z +
      3 * i * i * t * p1.z +
      3 * i * t * t * p2.z +
      t * t * t * p3.z;
    return out;
  }
}

class Circle {
  // Circle primitive to create circles.
  static generate(out, res, pX, pY, pZ, r) {
    // Takes input, out = vertex array, res = resolution of bezier curves of the circle, pX and pY and pZ are the coordinates of the center of the circle, r is the radius of circle.
    var top = new point4(pX, pY + r, pZ, 1), // First calculating top right left and bottom points by simply calculating center + radius.
      right = new point4(pX + r, pY, pZ, 1),
      left = new point4(pX - r, pY, pZ, 1),
      bottom = new point4(pX, pY - r, pZ, 1);
    // We generate 4 curves per circle. Going clockwise.
    Bezier.generate(
      out,
      res,
      top,
      this.getPerfectWeight(top, new Vec3().set(r, 0, 0)),
      this.getPerfectWeight(right, new Vec3().set(0, r, 0)),
      right
    );
    Bezier.generate(
      out,
      res,
      right,
      this.getPerfectWeight(right, new Vec3().set(0, -r, 0)),
      this.getPerfectWeight(bottom, new Vec3().set(r, 0, 0)),
      bottom
    );
    Bezier.generate(
      out,
      res,
      bottom,
      this.getPerfectWeight(bottom, new Vec3().set(-r, 0, 0)),
      this.getPerfectWeight(left, new Vec3().set(0, -r, 0)),
      left
    );
    Bezier.generate(
      out,
      res,
      left,
      this.getPerfectWeight(left, new Vec3().set(0, r, 0)),
      this.getPerfectWeight(top, new Vec3().set(-r, 0, 0)),
      top
    );
  }

  static getPerfectWeight(point, direction) {
    // This function is used to determine weight points of the bezier curve. The constant below comes from (4/3)*tan(pi/(2n)).
    direction.multi(0.552284749831);
    return new point4(
      point.x + direction.x,
      point.y + direction.y,
      point.z + direction.z,
      point.alpha
    );
  }
}

class Quad {
  // Basic Quad generator. It does not create 2 triangles instead creates a basic Quad directly.
  static generate(vPositions, a, b, c, d) {
    // It takes output array and 4 corners of the quad.
    var Index = 0;
    vPositions[Index] = a;
    Index++;
    vPositions[Index] = b;
    Index++;
    vPositions[Index] = c;
    Index++;
    vPositions[Index] = d;
    Index++;
  }
}
