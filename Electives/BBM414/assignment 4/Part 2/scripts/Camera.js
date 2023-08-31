/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

class Camera {
  // Perspective Camera Object
  constructor(fov, aspect, near, far) {
    // Setup the perspective matrix
    this.projectionMatrix = Matrix4.identity();
    Matrix4.perspective(
      this.projectionMatrix,
      (fov * Math.PI) / 180.0 || (45 * Math.PI) / 180.0,
      aspect,
      near || 0.1,
      far || 100.0
    );

    this.transform = new RenderObject(); //Setup transform to control the position of the camera
    this.viewMatrix = new Matrix4(); //Cache the matrix that will hold the inverse of the transform.
    this.target = new Vector3(0, 0, -1000);
    this.cameraMatrix = Matrix4.identity();
  }

  updateViewMatrix() {
    // We call lookat to calculate our view matrix.
    Matrix4.lookAt(
      this.viewMatrix,
      this.transform.position.getFloatArray(),
      this.target.getFloatArray(),
      [0, 1, 0]
    );

    // This was an old method.
    //this.viewMatrix.rotateX(this.transform.rotation.x);
    //this.viewMatrix.rotateY(this.transform.rotation.y);
    //this.viewMatrix.rotateZ(this.transform.rotation.z);

    Matrix4.invert(this.cameraMatrix, this.viewMatrix); // Camera matrix is inverse of view matrix.
    return this.cameraMatrix;
  }

  getTranslatelessMatrix() {
    // This is used for skybox renderer view matrix. It follows the camera.
    var mat = new Float32Array(this.viewMatrix.raw);
    mat[12] = mat[13] = mat[14] = 0.0; //Reset Translation position in the Matrix to zero.
    return mat;
  }

  getMatrix(name) {
    // Getting the matrix we need from the camera dynamically.
    if (name === "non_translate_matrix") {
      return this.getTranslatelessMatrix();
    } else if (name === "view_matrix") {
      return this.viewMatrix.raw;
    } else {
      console.error("This Matrix for camera is undefined: ", name);
      return null;
    }
  }
}
