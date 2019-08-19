import axios from "axios";

export default () =>
    axios.create({
        // baseURL: "http://localhost:5000"
        baseURL: "http://api.jamemy.ct8.pl"
    });
