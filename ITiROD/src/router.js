import { renderer } from './app.js';

export default class Router {

    #routes = {
        '/': 'MainPageView',
        '/welcome': 'WelcomePageView',
        '/404': '404'
    };

    get currentView() { 
        const path = window.location.pathname;
        console.log(path);
        return this.#routes[path] ? this.#routes[path] : this.#routes['/404'];
    }

    async navigate(path) {
        window.history.pushState(null, null, path);
        await this.render();
    }

    async render() {
        await renderer.render(this.currentView);
    }

    constructor() {
        window.history.replaceState(null, null, '/');
        window.addEventListener('popstate', async () => {
            await this.render();
        });
    }

}
