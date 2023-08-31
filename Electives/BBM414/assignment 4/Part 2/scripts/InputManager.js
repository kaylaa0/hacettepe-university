/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

class InputManager {
  // Input manager class which is used for keyboard and mouse input listeners, hooks and pointer lock management.
  static _canvas;
  static activeHooks = {}; // Currently active hooks. Key is hook name the value is callback function.
  static definedKeys = {}; // Currently defined keys. Key is key id the value is callback function.
  static checkPointerLock;
  static pointerLockStatus = false;
  static pointerLockInit = false;
  static rotateRate = -300;

  static _KeyboardEventHandler(event) {
    if (event.code === "KeyP") {
      // P is for default the pointer lock key.
      if (!InputManager.pointerLockInit) {
        InputManager._InitPointerLock();
      } else {
        if (InputManager.pointerLockStatus) {
          InputManager.unlockPointer();
        } else {
          InputManager.lockPointer();
        }
      }
    } else if (InputManager.pointerLockStatus) {
      if (typeof InputManager.definedKeys[event.code] !== "undefined") {
        // We iterate dictionary for matches.
        InputManager.definedKeys[event.code][0]();
      }
    }
  }

  static addKeyboardEventHandler(key, callback) {
    if (typeof InputManager.definedKeys[key] === "undefined") {
      // We iterate dictionary for matches.
      InputManager.definedKeys[key] = [];
    }
    InputManager.definedKeys[key].push(callback);
  }

  static hookEvent(event, callback) {
    if (typeof InputManager.activeHooks[event] === "undefined") {
      InputManager.activeHooks[event] = [];
    }
    InputManager.activeHooks[event].push(callback);
    window.addEventListener(event, callback, true);
  }

  static removeHook(event) {
    window.removeEventListener(event, InputManager.activeHooks[event], true);
    delete InputManager.activeHooks[event];
  }

  static removeAllHooks() {
    for (const [key, value] of Object.entries(InputManager.activeHooks)) {
      window.removeEventListener(key, value, true);
    }
  }

  static getHooks() {
    return InputManager.activeHooks;
  }

  static pointerLockMoveCallback(e) {
    // If we enter this for the first time, get the initial position.

    var movementx = e.movementX || e.mozmovementX || e.webkitmovementX || 0;

    var movementy = e.movementY || e.mozmovementY || e.webkitmovementY || 0;

    // Below is the FPS camera controller.
    // We calculate camera rotation to know current camera angle, which then used to determine if we pan down on x axis or z axis.
    // With in these functions the rotation degree is clipped.
    if (KaylaEngine.camera.transform.rotation.y > 180) {
      KaylaEngine.camera.transform.rotation.y = -179.9;
    }
    if (KaylaEngine.camera.transform.rotation.y < -180) {
      KaylaEngine.camera.transform.rotation.y = 179.9;
    }
    if (KaylaEngine.camera.transform.rotation.z < -90) {
      KaylaEngine.camera.transform.rotation.z = -90;
    }
    if (KaylaEngine.camera.transform.rotation.z > 90) {
      KaylaEngine.camera.transform.rotation.z = 90;
    }
    if (KaylaEngine.camera.transform.rotation.x > 90) {
      KaylaEngine.camera.transform.rotation.x = 90;
    }
    if (KaylaEngine.camera.transform.rotation.x < -90) {
      KaylaEngine.camera.transform.rotation.x = -90;
    }

    KaylaEngine.camera.target.rotateY((-movementx * Math.PI) / 180); // Rotating the target Y according to x movement which is basic.

    // Below we check the rotation degree and chose which axis to pan down on and the sign of movement.
    if (
      KaylaEngine.camera.transform.rotation.y <= -135 ||
      KaylaEngine.camera.transform.rotation.y >= 135
    ) {
      if (KaylaEngine.camera.transform.rotation.z !== 0) {
        KaylaEngine.camera.transform.rotation.x =
          KaylaEngine.camera.transform.rotation.z;
        KaylaEngine.camera.transform.rotation.z = 0;
      }
      KaylaEngine.camera.transform.rotation.y += movementx;
      KaylaEngine.camera.transform.rotation.x -=
        movementy * (InputManager.rotateRate / InputManager._canvas.height);
      KaylaEngine.camera.target.rotateX((movementy * Math.PI) / 180);
    } else if (
      KaylaEngine.camera.transform.rotation.y >= -45 &&
      KaylaEngine.camera.transform.rotation.y <= 45
    ) {
      if (KaylaEngine.camera.transform.rotation.z !== 0) {
        KaylaEngine.camera.transform.rotation.x =
          KaylaEngine.camera.transform.rotation.z;
        KaylaEngine.camera.transform.rotation.z = 0;
      }
      KaylaEngine.camera.transform.rotation.y += movementx;
      KaylaEngine.camera.transform.rotation.x +=
        movementy * (InputManager.rotateRate / InputManager._canvas.height);
      KaylaEngine.camera.target.rotateX((-movementy * Math.PI) / 180);
    } else if (
      KaylaEngine.camera.transform.rotation.y <= -45 &&
      KaylaEngine.camera.transform.rotation.y >= -135
    ) {
      if (KaylaEngine.camera.transform.rotation.x !== 0) {
        KaylaEngine.camera.transform.rotation.z =
          KaylaEngine.camera.transform.rotation.x;
        KaylaEngine.camera.transform.rotation.x = 0;
      }
      KaylaEngine.camera.transform.rotation.y += movementx;
      KaylaEngine.camera.transform.rotation.z +=
        movementy * (InputManager.rotateRate / InputManager._canvas.height);
      KaylaEngine.camera.target.rotateZ((movementy * Math.PI) / 180);
    } else if (
      KaylaEngine.camera.transform.rotation.y >= 45 &&
      KaylaEngine.camera.transform.rotation.y <= 135
    ) {
      if (KaylaEngine.camera.transform.rotation.x !== 0) {
        KaylaEngine.camera.transform.rotation.z =
          KaylaEngine.camera.transform.rotation.x;
        KaylaEngine.camera.transform.rotation.x = 0;
      }
      KaylaEngine.camera.transform.rotation.y += movementx;
      KaylaEngine.camera.transform.rotation.z -=
        movementy * (InputManager.rotateRate / InputManager._canvas.height);
      KaylaEngine.camera.target.rotateZ((-movementy * Math.PI) / 180);
    }
  }

  static _InitPointerLock() {
    // Initializes pointer lock.
    if (InputManager.pointerLockInit) {
      return;
    }
    if (InputManager.checkPointerLock) {
      InputManager.removeHook("click");
      InputManager._canvas.requestPointerLock =
        InputManager._canvas.requestPointerLock ||
        InputManager._canvas.mozRequestPointerLock ||
        InputManager._canvas.webkitRequestPointerLock;
      document.exitPointerLock =
        document.exitPointerLock ||
        document.mozExitPointerLock ||
        document.webkitExitPointerLock;
      InputManager.lockPointer();
      InputManager.pointerLockInit = true;
    } else {
      console.error("Pointer lock is not supported by current browser");
    }
  }

  static lockPointer() {
    InputManager._canvas.requestPointerLock();
    document.addEventListener(
      "mousemove",
      InputManager.pointerLockMoveCallback,
      false
    );
    document.removeEventListener("click", InputManager.lockPointer, false);
  }

  static unlockPointer() {
    document.exitPointerLock();
    document.removeEventListener(
      "mousemove",
      InputManager.pointerLockMoveCallback,
      false
    );
  }

  static pointerLockChangeCallback(e) {
    if (InputManager.pointerLockStatus) {
      console.log("Pointer lock exited.");
      InputManager.pointerLockStatus = false;
      document.addEventListener("click", InputManager.lockPointer, false);
      document.removeEventListener(
        "mousemove",
        InputManager.pointerLockMoveCallback,
        false
      );
    } else {
      InputManager.pointerLockStatus = true;
    }
  }

  static _Init() {
    InputManager._canvas = document.getElementById("canvas");

    InputManager.checkPointerLock =
      "pointerLockElement" in document ||
      "mozPointerLockElement" in document ||
      "webkitPointerLockElement" in document;

    document.addEventListener(
      "pointerlockchange",
      InputManager.pointerLockChangeCallback
    );
    document.addEventListener(
      "mozpointerlockchange",
      InputManager.pointerLockChangeCallback
    );
    document.addEventListener(
      "webkitpointerlockchange",
      InputManager.pointerLockChangeCallback
    );

    document.addEventListener("keydown", InputManager._KeyboardEventHandler);
  }
}
