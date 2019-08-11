const express = require("express");
const bodyParser = require("body-parser");
const path = require("path");
const mysql = require("mysql");

const app = express();

const port = 5000;

const db = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "",
    database: "jamemy"
});

db.connect((err) => {
    if (err) {
        throw err;
    }

    console.log("Connected to database!");
});

app.set("port", port);
app.set("views", path.join(__dirname, "views"));
app.set("view engine", "ejs");

app.use(bodyParser.urlencoded({ extended:false }));
app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, 'public')));

app.get("/", (req, res) => {
    let query = "SELECT author.name, meme.image_url FROM meme JOIN author ON author.id = meme.author_id;";

    db.query(query, (err, result) => {
        if (err) {
            throw err;
        }

        res.render("page.ejs", {
            memes: result
        });
    });

   
});

app.listen(port, () => {
    console.log(`Server running on port: ${port}`);
});
