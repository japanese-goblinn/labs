import IndexRenderer from './views/mainPage/indexRenderer.js';
import WelcomePageRenderer from './views/welcomePage/welcomePageRenderer.js';

export default class Router {

    #routes = {
        '/': () => { 
            document.styleSheets[0].disabled = false;
            document.styleSheets[1].disabled = true;
            return new IndexRenderer() 
        },
        '/welcome': () => { 
            document.styleSheets[0].disabled = true;
            document.styleSheets[1].disabled = false;
            return new WelcomePageRenderer() 
        }
    };

    get currentView() { 
        const path = window.location.pathname;
        return this.#routes[path] ? this.#routes[path] : "not founded";
    }

    async navigate(path) {
        window.history.pushState(null, null, path);
        await this.render()
    }

    async render() {
        const view = this.currentView();
        await view.render();
    }

    constructor() {
        window.history.replaceState(null, null, '/');
        window.addEventListener('popstate', async () => {
            await this.render();
        });
    }

}
