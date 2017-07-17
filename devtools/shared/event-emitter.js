/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

(function (factory) {
  // This file can be loaded in several different ways.  It can be
  // require()d, either from the main thread or from a worker thread;
  // or it can be imported via Cu.import.  These different forms
  // explain some of the hairiness of this code.
  //
  // It's important for the devtools-as-html project that a require()
  // on the main thread not use any chrome privileged APIs.  Instead,
  // the body of the main function can only require() (not Cu.import)
  // modules that are available in the devtools content mode.  This,
  // plus the lack of |console| in workers, results in some gyrations
  // in the definition of |console|.
  if (this.module && module.id.indexOf("event-emitter") >= 0) {
    let console;
    if (isWorker) {
      console = {
        error: () => {}
      };
    } else {
      console = this.console;
    }
    // require
    factory.call(this, require, exports, module, console);
  } else {
    // Cu.import.  This snippet implements a sort of miniature loader,
    // which is responsible for appropriately translating require()
    // requests from the client function.  This code can use
    // Cu.import, because it is never run in the devtools-in-content
    // mode.
    this.isWorker = false;
    const Cu = Components.utils;
    let console = Cu.import("resource://gre/modules/Console.jsm", {}).console;
    // Bug 1259045: This module is loaded early in firefox startup as a JSM,
    // but it doesn't depends on any real module. We can save a few cycles
    // and bytes by not loading Loader.jsm.
    let require = function (module) {
      switch (module) {
        case "Services":
          return Cu.import("resource://gre/modules/Services.jsm", {}).Services;
        case "devtools/shared/platform/stack": {
          let obj = {};
          Cu.import("resource://devtools/shared/platform/chrome/stack.js", obj);
          return obj;
        }
      }
      return null;
    };
    factory.call(this, require, this, { exports: this }, console);
    this.EXPORTED_SYMBOLS = ["EventEmitter"];
  }
}).call(this, function (require, exports, module, console) {
  // ⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠
  // After this point the code may not use Cu.import, and should only
  // require() modules that are "clean-for-content".

  const BAD_LISTENER = "The event listener must be a function.";

  const eventListeners = Symbol("EventEmitter/listeners");
  const originalListener = Symbol("EventEmitter/original-listener");

  class EventEmitter {
    constructor() {
      this[eventListeners] = new Map();
    }

    /**
     * Registers an event `listener` that is called every time events of
     * specified `type` is emitted on the given event `target`.
     *
     * @param {Object} target
     *    Event target object.
     * @param {String} type
     *    The type of event.
     * @param {Function} listener
     *    The listener function that processes the event.
     */
    static on(target, type, listener) {
      if (typeof listener !== "function") {
        throw new Error(BAD_LISTENER);
      }

      if (!(eventListeners in target)) {
        target[eventListeners] = new Map();
      }

      let events = target[eventListeners];

      if (events.has(type)) {
        events.get(type).add(listener);
      } else {
        events.set(type, new Set([listener]));
      }
    }

    /**
     * Removes an event `listener` for the given event `type` on the given event
     * `target`. If no `listener` is passed removes all listeners of the given
     * `type`. If `type` is not passed removes all the listeners of the given
     * event `target`.
     * @param {Object} target
     *    The event target object.
     * @param {String} type
     *    The type of event.
     * @param {Function} listener
     *    The listener function that processes the event.
     */
    static off(target, type, listener) {
      let length = arguments.length;
      let events = target[eventListeners];

      if (!events) {
        return;
      }

      if (length === 3) {
        let listenersForType = events.get(type);

        if (listenersForType) {
          if (listenersForType.has(listener)) {
            listenersForType.delete(listener);
          } else {
            for (let value of listenersForType.values()) {
              if (originalListener in value && value[originalListener] === listener) {
                listenersForType.delete(value);
                return;
              }
            }
          }
        }
      } else if (length === 2) {
        if (events.has(type)) {
          events.get(type).clear();
          events.delete(type);
        }
      } else if (length === 1) {
        for (let listeners of events.values()) {
          listeners.clear();
          events.delete(type);
        }
      }
    }

    /**
     * Registers an event `listener` that is called only the next time an event
     * of the specified `type` is emitted on the given event `target`.
     * @param {Object} target
     *    Event target object.
     * @param {String} type
     *    The type of the event.
     * @param {Function} listener
     *    The listener function that processes the event.
     */
    static once(target, type, listener) {
      return new Promise(resolve => {
        let handler = (first, ...rest) => {
          EventEmitter.off(target, type, handler);
          if (listener) {
            listener(first, ...rest);
          }
          resolve(first);
        };

        handler[originalListener] = listener;
        EventEmitter.on(target, type, handler);
      });
    }

    static emit(target, type, ...rest) {
      logEvent(type, rest);

      if (!(eventListeners in target) || !target[eventListeners].has(type)) {
        return;
      }

      // Creating a temporary Set with the original listeners, to avoiding side effects
      // in emit.
      let listenersForType = new Set(target[eventListeners].get(type));

      for (let listener of listenersForType) {
        // If the object was destroyed during event emission, stop
        // emitting.
        if (!(eventListeners in target)) {
          break;
        }

        // If listeners were removed during emission, make sure the
        // event handler we're going to fire wasn't removed.
        if (target[eventListeners].get(type) &&
          target[eventListeners].get(type).has(listener)) {
          try {
            listener.call(target, ...rest);
          } catch (ex) {
            // Prevent a bad listener from interfering with the others.
            let msg = ex + ": " + ex.stack;
            console.error(msg);
            dump(msg + "\n");
          }
        }
      }
    }

    /**
     * Returns a number of event listeners registered for the given event `type`
     * on the given event `target`.
     *
     * @param {Object} target
     *    Event target object.
     * @param {String} type
     *    The type of event.
     * @return {Number}
     *    The number of event listeners.
     */
    static count(target, type) {
      if (eventListeners in target) {
        let listenersForType = target[eventListeners].get(type);

        if (listenersForType) {
          return listenersForType.size;
        }
      }

      return 0;
    }

    /**
     * Decorate an object with event emitter functionality; basically using the
     * class' prototype as mixin.
     *
     * @param Object target
     *    The object to decorate.
     * @return Object
     *    The object given, mixed.
     */
    static decorate(target) {
      let descriptors = Object.getOwnPropertyDescriptors(this.prototype);
      delete descriptors.constructor;
      return Object.defineProperties(target, descriptors);
    }

    on(...args) {
      EventEmitter.on(this, ...args);
    }

    off(...args) {
      EventEmitter.off(this, ...args);
    }

    once(...args) {
      return EventEmitter.once(this, ...args);
    }

    emit(...args) {
      EventEmitter.emit(this, ...args);
    }
  }

  module.exports = this.EventEmitter = EventEmitter;

  // See comment in JSM module boilerplate when adding a new dependency.
  const Services = require("Services");
  const { describeNthCaller } = require("devtools/shared/platform/stack");
  let loggingEnabled = true;

  if (!isWorker) {
    loggingEnabled = Services.prefs.getBoolPref("devtools.dump.emit");
    Services.prefs.addObserver("devtools.dump.emit", {
      observe: () => {
        loggingEnabled = Services.prefs.getBoolPref("devtools.dump.emit");
      }
    });
  }

  function serialize(target) {
    let out = String(target);

    if (target && target.nodeName) {
      out += " (" + target.nodeName;
      if (target.id) {
        out += "#" + target.id;
      }
      if (target.className) {
        out += "." + target.className;
      }
      out += ")";
    }

    return out;
  }

  function logEvent(type, args) {
    if (!loggingEnabled) {
      return;
    }

    let argsOut = "";
    let description = describeNthCaller(2);

    // We need this try / catch to prevent any dead object errors.
    try {
      argsOut = args.map(serialize).join(", ");
    } catch (e) {
      // Object is dead so the toolbox is most likely shutting down,
      // do nothing.
    }

    dump(`EMITTING: emit(${type}${argsOut}) from ${description}\n`);
  }
});
