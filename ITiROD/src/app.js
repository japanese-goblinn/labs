import Router from './router.js';
import Renderer from './renderer.js';

export const mobile = window.matchMedia('screen and (max-width: 800px)');
export const renderer = new Renderer();
export const router = new Router();
window.addEventListener('load', router.render());
