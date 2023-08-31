/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

/*
 You can use arrow or WASD for movement, movement is more advanced than how it wanted in the PDF. I spend lots of time to construct this and this is the way for a FPS movement.
 Space or PageUp goes up.
 C key or PageDown goes down.
 Plus rotates faster.
 Minus rotates slower.
 Clicking or P key toggles pointer lock.
 When locked moving mouse moves camera as in FPS camera movement. Uses Perspective Camera with lookat function.
 When window is scaled the viewport scales so the view is never distorted.
 There also is Skybox and Lighting as a bonus.
*/

var gl; // Our main gl variable

var LOADED_IMG_COUNT = 0; // With this variable we check if the images for skybox is loaded. If it is we construct and add skybox to scene.

class KaylaEngine {
  // My main engine class
  static objects = []; // We store current objects in scene here so we render them later by calling render withing each.
  static shaders = {}; // Main shader database for easier access.
  static camera; // Our Scene Camera is also stored for access.

  static animationRotationScale = 1; // The scale which the monkey head rotates.

  static _Init() {
    Renderer._Init(); // Initializing renderer.

    gl = Renderer.setUpGL("canvas"); // Setting up canvas
    gl._SetSize(
      Renderer._constants["resolution"][0],
      Renderer._constants["resolution"][1]
    ); // Setting up initial size.
    gl._Clear(); // Clearing canvas for the first time.

    window.addEventListener("resize", Renderer.ResizeWindow, false); // Adding event listener to window resize, so when window is resized the viewport also gets resized.

    KaylaEngine.camera = new Camera(
      60,
      Renderer._constants["resolution"][0] /
        Renderer._constants["resolution"][1],
      0.1,
      1000
    ); // Creating a perspective camera as our Scene Camera.
    KaylaEngine.camera.transform.position.set(0, 1, 4); // Setting up position of our camera.
    InputManager._Init(Renderer.GL, KaylaEngine.camera); // Initializing InputManager class.

    //Loading up images
    KaylaEngine.loadImage("resources/skybox/back.jpg", "skybox_front");
    KaylaEngine.loadImage("resources/skybox/front.jpg", "skybox_back");
    KaylaEngine.loadImage("resources/skybox/bottom.jpg", "skybox_bottom");
    KaylaEngine.loadImage("resources/skybox/left.jpg", "skybox_left");
    KaylaEngine.loadImage("resources/skybox/right.jpg", "skybox_right");
    KaylaEngine.loadImage("resources/skybox/top.jpg", "skybox_top");
    setTimeout(KaylaEngine.waitForLoaded, 100); // Waiting images to load.

    KaylaEngine.shaders["default_shader"] = new DefaultShader(
      Renderer.GL,
      KaylaEngine.camera.projectionMatrix
    ); // Setting up default shader.

    KaylaEngine.readFromFileToObject(
      "resources/monkey_head.obj",
      "monkeyHead",
      "default_shader", // Reading from file to object. First monkey head then ground.
      "view_matrix",
      {
        colorOverRide: [0.8, 0.8, 0.8], // Color gray
      },
      [0, 0, 0],
      [0.5, 0.5, 0.5],
      [0, 0, 0],
      function () {
        this.addRotation(0, KaylaEngine.animationRotationScale, 0); // Animation function of the monkey head. It is set dynamically. It read from KaylaEngine.animationRotationScale. When we update, the rotation will change.
      }
    );
    KaylaEngine.readFromFileToObject(
      "resources/ground.obj",
      "groundMesh",
      "default_shader",
      "view_matrix",
      {
        colorOverRide: [0.5, 1, 0], // Color green
      },
      [-20, -1, -10],
      [400, 1, 200],
      [0, 0, 0]
    );

    // Setting up inputs.
    InputManager.hookEvent("click", InputManager._InitPointerLock); // Clicking for pointerLock. You can also use P key.
    // WASD or arrow keys for movement. Space/C or PageUp/PageDown for up and down movement. Plus/Minus for head rotation speed.
    InputManager.addKeyboardEventHandler("KeyW", KaylaEngine.MoveForward);
    InputManager.addKeyboardEventHandler("KeyS", KaylaEngine.MoveBackward);
    InputManager.addKeyboardEventHandler("KeyD", KaylaEngine.MoveRight);
    InputManager.addKeyboardEventHandler("KeyA", KaylaEngine.MoveLeft);
    InputManager.addKeyboardEventHandler("Space", KaylaEngine.MoveUp);
    InputManager.addKeyboardEventHandler("KeyC", KaylaEngine.MoveDown);
    InputManager.addKeyboardEventHandler("ArrowUp", KaylaEngine.MoveForward);
    InputManager.addKeyboardEventHandler("ArrowDown", KaylaEngine.MoveBackward);
    InputManager.addKeyboardEventHandler("ArrowRight", KaylaEngine.MoveRight);
    InputManager.addKeyboardEventHandler("ArrowLeft", KaylaEngine.MoveLeft);
    InputManager.addKeyboardEventHandler("PageUp", KaylaEngine.MoveUp);
    InputManager.addKeyboardEventHandler("PageDown", KaylaEngine.MoveDown);
    InputManager.addKeyboardEventHandler("Plus", KaylaEngine.rotationFaster);
    InputManager.addKeyboardEventHandler(
      "NumpadAdd",
      KaylaEngine.rotationFaster
    );
    InputManager.addKeyboardEventHandler(
      "NumpadSubtract",
      KaylaEngine.rotationSlower
    );
    InputManager.addKeyboardEventHandler("Minus", KaylaEngine.rotationSlower);

    KaylaEngine._AnimationLoop(); // Starting the animation loop.
  }

  static waitForLoaded = function () {
    // Waiting for the images to load if all 6 are loaded, we construct the skybox object.
    if (LOADED_IMG_COUNT === 6) {
      Renderer.GL.fLoadCubeMap("skybox01", [
        document.getElementById("skybox_right"),
        document.getElementById("skybox_left"),
        document.getElementById("skybox_top"),
        document.getElementById("skybox_bottom"),
        document.getElementById("skybox_back"),
        document.getElementById("skybox_front"),
      ]);
      KaylaEngine.shaders["textured_shader"] = new SkyBoxShader(
        Renderer.GL,
        KaylaEngine.camera.projectionMatrix,
        Renderer.GL.mTextureCache["skybox01"]
      ); // Skybox has special shader, it does not get affected by lighting and its position follows camera.
      KaylaEngine.insertObject(
        new Mesh(
          Shapes3D.Cube.createMesh(
            Renderer.GL,
            "SkyBox",
            100,
            100,
            100,
            0,
            0,
            0
          ),
          "textured_shader",
          "non_translate_matrix"
        ),
        false
      ); // Inserting skybox to scene.
    } else {
      // Recalling until all is loaded.
      setTimeout(KaylaEngine.waitForLoaded, 100); // Try again in 100ms.
    }
  };

  static rotationFaster() {
    // Increasing monkey head rotation speed.
    KaylaEngine.animationRotationScale += 0.25;
  }

  static rotationSlower() {
    // Decreasing monkey head rotation speed.
    KaylaEngine.animationRotationScale -= 0.25;
  }

  static MoveForward() {
    // Basically there is a lookup object and we get direction of it and we add it to move in the direction we are looking at.
    KaylaEngine.camera.transform.position.x +=
      KaylaEngine.camera.target.getNormalized()[0] / 10;
    KaylaEngine.camera.transform.position.z +=
      KaylaEngine.camera.target.getNormalized()[2] / 10;
  }

  static MoveBackward() {
    // We subtract it to move in the opposite direction we are looking at.
    KaylaEngine.camera.transform.position.x -=
      KaylaEngine.camera.target.getNormalized()[0] / 10;
    KaylaEngine.camera.transform.position.z -=
      KaylaEngine.camera.target.getNormalized()[2] / 10;
  }

  static MoveRight() {
    // First we rotate look at vector to 90 degrees. This does not actually changes it just gives us a result.
    // We subtract it to move in that direction which is right of our camera.
    KaylaEngine.camera.transform.position.x -=
      KaylaEngine.camera.target.getRight()[0] / 10;
    KaylaEngine.camera.transform.position.z -=
      KaylaEngine.camera.target.getRight()[2] / 10;
  }

  static MoveLeft() {
    // We rotate look at vector to -90 degrees. This does not actually changes it just gives us a result.
    // We subtract it to move in that direction which is left of our camera.
    KaylaEngine.camera.transform.position.x -=
      KaylaEngine.camera.target.getLeft()[0] / 10;
    KaylaEngine.camera.transform.position.z -=
      KaylaEngine.camera.target.getLeft()[2] / 10;
  }

  static MoveUp() {
    // Moving up is more basic since our up is always +y, we just add to it.
    KaylaEngine.camera.transform.position.y += 0.1;
  }

  static MoveDown() {
    // We subtract from y to move down.
    KaylaEngine.camera.transform.position.y -= 0.1;
  }

  static imgLoadHandler() {
    // When an image is loaded the count is incremented. If it is total 6 means all 6 sides of the skybox loaded we move on.
    LOADED_IMG_COUNT += 1;
  }

  static loadImage(src, id) {
    // Loads an image.
    let img = new Image(); // First we create image object.

    img.id = id; // Set it's id and src.
    img.src = src;
    img.style.display = "none"; // This is to ensure it is not displayed actually on screen

    img.addEventListener("load", KaylaEngine.imgLoadHandler); // When it is loaded we increment counter. This takes time.

    document.body.append(img); // Add it to html body.
  }

  static readFromFileToObject(
    fileName,
    objName,
    shader,
    renderMatrixName,
    shaderVariables,
    position,
    scale,
    rotation,
    objFunction
  ) {
    // Reads from .obj file and converts it to an object and adds it to scene. All automatically.
    let readFile = new XMLHttpRequest(); // We read files with XMLHTTPRequest.
    readFile.open("GET", fileName, false); // Getting File.
    readFile.onreadystatechange =
      function () // When the file is read this will be triggered. It takes time.
      {
        if (readFile.readyState === 4) {
          // We check again if the file is read.
          if (readFile.status === 200 || readFile.status === 0) {
            // We check status.
            let fileText = readFile.responseText; // We get text data from the file.
            // And we insert the object to the Scene.
            KaylaEngine.insertObject(
              new Mesh(
                TextConverter.fileToMesh(objName, fileText, true),
                shader,
                renderMatrixName,
                shaderVariables,
                objFunction
              ),
              true,
              position,
              scale,
              rotation
            );
          }
        }
      };
    readFile.send(null); // We send to file to trigger it.
  }

  static insertObject(object, set, position, scale, rotation) {
    // Inserts objects to scene.
    if (set) {
      // If this is set it means the position, scale, rotation variables are to be set.
      object
        .setPosition(position[0], position[1], position[2])
        .setScale(scale[0], scale[1], scale[2])
        .setRotation(rotation[0], rotation[1], rotation[2]);
    }

    KaylaEngine.objects.push(object); // We add object to our array.
  }

  static _AnimationLoop() {
    // Animation loop.
    // With this method we can call it 60 times per second.
    requestAnimationFrame((t) => {
      KaylaEngine._AnimationLoop(); // We iterate.

      KaylaEngine.camera.updateViewMatrix(); // Update camera matrix.
      Renderer.GL._Clear(); // Clear the canvas.

      KaylaEngine.objects.forEach(Renderer.renderObject); // For each object in our scene render it.
    });
  }
}

window.addEventListener("load", () => {
  // After the content of html loaded we call the main function. This helps with resolution problems and other problems such as styling etc.
  // It must be after the load.
  KaylaEngine._Init();
});
