import { firebaseConfig } from './scripts/firebaseConfig.js';

import Router from './scripts/router.js';
import Renderer from './scripts/renderer.js';

firebase.initializeApp(firebaseConfig);
export const mobile = window.matchMedia('screen and (max-width: 800px)');
export const renderer = new Renderer();
export const router = new Router();
