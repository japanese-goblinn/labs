import Auth from "./auth.js";

export default class Database {
    
    static init() {
        this.db = firebase.database();
    }
}
