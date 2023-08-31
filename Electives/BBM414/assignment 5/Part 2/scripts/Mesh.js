/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

class Mesh {
  // Mesh object.
  constructor(
    meshData,
    shaderName,
    renderMatrixName,
    shaderVariables,
    frameFunction
  ) {
    this.name = meshData.nameTag;
    this.transform = new RenderObject();
    this.mesh = meshData;
    this.frameFunction = frameFunction;
    this.shaderName = shaderName;
    this.renderMatrixName = renderMatrixName;
    this.shaderVariables = shaderVariables;
  }

  setScale(x, y, z) {
    // Setting Scale
    this.transform.scale.set(x, y, z);
    return this;
  }

  setPosition(x, y, z) {
    // Setting Position
    this.transform.position.set(x, y, z);
    return this;
  }

  setRotation(x, y, z) {
    // Setting Rotation
    this.transform.rotation.set(x, y, z);
    return this;
  }

  addRotation(x, y, z) {
    // Adding Rotation
    this.transform.rotation.x += x;
    this.transform.rotation.y += y;
    this.transform.rotation.z += z;
    return this;
  }

  // Calculate before rendering
  preRender() {
    if (typeof this.frameFunction === "function") {
      this.frameFunction();
    }
    this.transform.updateMatrix();
    return this;
  }
}

class RenderObject {
  constructor() {
    //transform vectors
    this.position = new Vector3(0, 0, 0); //Traditional X,Y,Z 3d position
    this.scale = new Vector3(1, 1, 1); //How much to scale a mesh. Having a 1 means no scaling is done.
    this.rotation = new Vector3(0, 0, 0); //Hold rotation values based on degrees, Object will translate it to radians
    this.matView = new Matrix4(); //Cache the results when calling updateMatrix
    this.matNormal = new Float32Array(9); //This is a Mat3, raw array to hold the values is enough for what its used for
  }

  //Methods
  updateMatrix() {
    this.matView
      .reset()
      .vtranslate(this.position)
      .rotateX(this.rotation.x * RenderObject.deg2Rad)
      .rotateZ(this.rotation.z * RenderObject.deg2Rad)
      .rotateY(this.rotation.y * RenderObject.deg2Rad)
      .vscale(this.scale);

    //Calculate the Normal Matrix
    Matrix4.normalMat3(this.matNormal, this.matView.raw);

    return this.matView.raw;
  }

  getViewMatrix() {
    return this.matView.raw;
  }
}

RenderObject.deg2Rad = Math.PI / 180;
