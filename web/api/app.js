const express = require("express");
const cors = require("cors");
const mysql = require("mysql");
const fs = require("fs");

const app = express();

const port = 5000;

let config = JSON.parse(fs.readFileSync("config.json"));

const db = mysql.createConnection({
    host: config.host,
    user: config.user,
    password: config.password,
    database: config.database
});

db.connect((err) => {
    if (err) {
        throw err;
    }

    console.log("Connected to database!");
});

app.use(cors());
app.set("port", port);

app.get("/today", cors(), (req, res) => {
    let where = "WHERE meme.date BETWEEN curdate() and curdate() + interval 24 hour";

    sendData(where, res);
});

app.get("/yesterday", cors(), (req, res) => {
    let where = "WHERE meme.date BETWEEN curdate() - interval 1 day and curdate()";

    sendData(where, res);
});

app.get("/last14days", cors(), (req, res) => {
    let where = "WHERE meme.date BETWEEN curdate() - interval 14 day and curdate()";

    sendData(where, res);
});


app.get("/month/:month/year/:year", (req, res) => {
    let where = `
    WHERE meme.date BETWEEN
    "${req.params.year}-${req.params.month}-01" and 
    "${req.params.year}-${req.params.month}-01" + interval 1 month - interval 1 second`;

    sendData(where, res);
});

app.get("/year/:year", (req, res) => {
    let where = `
    WHERE meme.date BETWEEN
    "${req.params.year}-01-01" and 
    "${req.params.year}-01-01" + interval 12 month - interval 1 second`;

    sendData(where, res);
});

app.get("/between/:start/:end", (req, res) => {
    let where = `
    WHERE meme.date BETWEEN
    "${req.params.start}" and 
    "${req.params.end}"`;

    sendData(where, res);
});

app.listen(port, () => {
    console.log(`Server running on port: ${port}`);
});

function sendData(dateFilter, res)
{
    let query = `SELECT author.name, meme.image_url, meme.karma, meme.date
    FROM meme 
    JOIN author 
        ON author.id = meme.author_id
    ${dateFilter}
    ORDER BY meme.karma DESC`;

    db.query(query, (err, result) => {
        if (err) {
            throw err;
        }

        res.status(200).send(result);
    });
}
