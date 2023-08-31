/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

/*
 You can use arrow or WASD for movement.
 Space or PageUp goes up.
 C key or PageDown goes down.
 O toggles spotlight.
 P Changes vertex or fragment shader.
 When locked moving mouse moves camera as in FPS camera movement. Uses Perspective Camera with lookat function.
 When window is scaled the viewport scales so the view is never distorted.
 There is also Skybox.
*/

var gl; // Our main gl variable

var LOADED_IMG_COUNT = 0; // With this variable we check if the images for skybox is loaded. If it is we construct and add skybox to scene.

class KaylaEngine {
  // My main engine class
  static objects = []; // We store current objects in scene here so we render them later by calling render withing each.
  static shaders = {}; // Main shader database for easier access.
  static camera; // Our Scene Camera is also stored for access.
  static toggleOff = 0.0; // Passed to all shaders if 1.0 means toggled off.
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
      10000
    ); // Creating a perspective camera as our Scene Camera.
    KaylaEngine.camera.transform.position.set(0, 10, 4); // Setting up position of our camera.

    InputManager._Init(Renderer.GL, KaylaEngine.camera); // Initializing InputManager class.

    KaylaEngine.camera.transform.rotation.y += -45;
    KaylaEngine.camera.transform.rotation.x -=
      -45 * (InputManager.rotateRate / InputManager._canvas.height);
    KaylaEngine.camera.target.rotateX((-45 * Math.PI) / 180);

    //Loading up images
    KaylaEngine.loadImage("resources/skybox/back.jpg", "skybox_front");
    KaylaEngine.loadImage("resources/skybox/front.jpg", "skybox_back");
    KaylaEngine.loadImage("resources/skybox/bottom.jpg", "skybox_bottom");
    KaylaEngine.loadImage("resources/skybox/left.jpg", "skybox_left");
    KaylaEngine.loadImage("resources/skybox/right.jpg", "skybox_right");
    KaylaEngine.loadImage("resources/skybox/top.jpg", "skybox_top");
    KaylaEngine.loadImage("resources/emoji.png", "emoji");
    KaylaEngine.loadImage("resources/grass.jpg", "grass");
    setTimeout(KaylaEngine.waitForLoaded, 100); // Waiting images to load.

    KaylaEngine.shaders["default_shader"] = new DefaultShader(
      Renderer.GL,
      KaylaEngine.camera.projectionMatrix
    ); // Setting up default shader.

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
    InputManager.addKeyboardEventHandler("KeyO", KaylaEngine.toggleSpotLight);
    InputManager.addKeyboardEventHandler("KeyP", KaylaEngine.switchShaders);

    KaylaEngine._AnimationLoop(); // Starting the animation loop.
  }

  static waitForLoaded = function () {
    // Waiting for the images to load if all 6 are loaded, we construct the skybox object.
    if (LOADED_IMG_COUNT === 8) {
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

      // Loading textures for this part.
      Renderer.GL.fLoadTexture(
        "emoji",
        document.getElementById("emoji"),
        false
      );
      Renderer.GL.fLoadTexture(
        "grass",
        document.getElementById("grass"),
        false
      );

      // Creating shaders for vertex type spot light and fragment type spotlight.
      KaylaEngine.shaders["textured_light_shader"] = new TextureLightShader(
        Renderer.GL,
        KaylaEngine.camera.projectionMatrix,
        Renderer.GL.mTextureCache["emoji"]
      );
      KaylaEngine.shaders["textured_light_fragment_shader"] =
        new TextureLightFragmentShader(
          Renderer.GL,
          KaylaEngine.camera.projectionMatrix,
          Renderer.GL.mTextureCache["emoji"]
        );

      KaylaEngine.shaders["textured_light_shader_ground"] =
        new TextureLightShader(
          Renderer.GL,
          KaylaEngine.camera.projectionMatrix,
          Renderer.GL.mTextureCache["grass"]
        );
      KaylaEngine.shaders["textured_light_fragment_shader_ground"] =
        new TextureLightFragmentShader(
          Renderer.GL,
          KaylaEngine.camera.projectionMatrix,
          Renderer.GL.mTextureCache["grass"]
        );

      KaylaEngine.insertObject(
        new Mesh(
          Shapes3D.Cube.createMesh(
            Renderer.GL,
            "SkyBox",
            1000,
            1000,
            1000,
            0,
            0,
            0
          ),
          "textured_shader",
          "non_translate_matrix"
        ),
        false
      ); // Inserting skybox to scene.

      // Inserting mask emoji boxes to scene from -24 to 24 with 2 steps. Should be a 25x25 grid and total to 625.
      var boxID = 0;
      for (var j = -24; j <= 24; j += 2) {
        for (var i = -24; i <= 24; i += 2) {
          KaylaEngine.insertObject(
            new Mesh(
              Shapes3D.Cube.createMesh(
                Renderer.GL,
                "Box" + boxID,
                1,
                1,
                1,
                0,
                0,
                0,
                1
              ),
              "textured_light_fragment_shader",
              "view_matrix"
            ).setPosition(j, 0, -i),
            false
          );
          boxID++;
        }
      }

      // Normally ground was 1 big box with repeating textures. However, in order to get effect similar to TA's vertex shader video, I needed to craft a couple of medium-sized boxes.
      var groundID = 0;
      for (var j = -50; j <= 50; j += 3) {
        for (var i = -50; i <= 50; i += 3) {
          KaylaEngine.insertObject(
            new Mesh(
              Shapes3D.Cube.createMesh(
                Renderer.GL,
                "Ground" + groundID,
                3,
                0.1,
                3,
                0,
                0,
                0,
                3
              ),
              "textured_light_fragment_shader_ground",
              "view_matrix"
            ).setPosition(j, -0.501, i),
            false
          );
          groundID++;
        }
      }
    } else {
      // Recalling until all is loaded.
      setTimeout(KaylaEngine.waitForLoaded, 100); // Try again in 100ms.
    }
  };

  static toggleSpotLight() {
    // Passed to all shaders if 1.0 means toggled off.
    if (KaylaEngine.toggleOff === 0.0) {
      KaylaEngine.toggleOff = 1.0;
    } else if (KaylaEngine.toggleOff === 1.0) {
      KaylaEngine.toggleOff = 0.0;
    } else {
      console.error("Toggle value should be 0.0 or 1.0");
    }
  }

  static switchShader(object) {
    // We check if object is type that needs to be changed. Skybox etc. does not need to change.
    if (object.name.startsWith("Box")) {
      // And we change the shader name. Everything will be handled automatically in renderers.
      if (object.shaderName === "textured_light_shader") {
        object.shaderName = "textured_light_fragment_shader";
      } else if (object.shaderName === "textured_light_fragment_shader") {
        object.shaderName = "textured_light_shader";
      }
    } else if (object.name.startsWith("Ground")) {
      if (object.shaderName === "textured_light_shader_ground") {
        object.shaderName = "textured_light_fragment_shader_ground";
      } else if (
        object.shaderName === "textured_light_fragment_shader_ground"
      ) {
        object.shaderName = "textured_light_shader_ground";
      }
    }
  }

  static switchShaders() {
    // For each object we call switch shader function.
    KaylaEngine.objects.forEach(KaylaEngine.switchShader);
  }

  static MoveForward() {
    // Basically there is a lookup object and we get direction of it and we add it to move in the direction we are looking at.
    KaylaEngine.camera.transform.position.x +=
      KaylaEngine.camera.target.getNormalized()[0] / 5;
    KaylaEngine.camera.transform.position.z +=
      KaylaEngine.camera.target.getNormalized()[2] / 5;
  }

  static MoveBackward() {
    // We subtract it to move in the opposite direction we are looking at.
    KaylaEngine.camera.transform.position.x -=
      KaylaEngine.camera.target.getNormalized()[0] / 5;
    KaylaEngine.camera.transform.position.z -=
      KaylaEngine.camera.target.getNormalized()[2] / 5;
  }

  static MoveRight() {
    // First we rotate look at vector to 90 degrees. This does not actually changes it just gives us a result.
    // We subtract it to move in that direction which is right of our camera.
    KaylaEngine.camera.transform.position.x -=
      KaylaEngine.camera.target.getRight()[0] / 5;
    KaylaEngine.camera.transform.position.z -=
      KaylaEngine.camera.target.getRight()[2] / 5;
  }

  static MoveLeft() {
    // We rotate look at vector to -90 degrees. This does not actually changes it just gives us a result.
    // We subtract it to move in that direction which is left of our camera.
    KaylaEngine.camera.transform.position.x -=
      KaylaEngine.camera.target.getLeft()[0] / 5;
    KaylaEngine.camera.transform.position.z -=
      KaylaEngine.camera.target.getLeft()[2] / 5;
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
      //console.log("1", KaylaEngine.camera.transform.position.x,KaylaEngine.camera.transform.position.y,KaylaEngine.camera.transform.position.z);
      //console.log("2", KaylaEngine.camera.cameraMatrix[12],KaylaEngine.camera.cameraMatrix[13],KaylaEngine.camera.cameraMatrix[14], KaylaEngine.camera.cameraMatrix[15]);
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
