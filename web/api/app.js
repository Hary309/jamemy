const express = require("express");
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

app.get("/today", (req, res) => {
    let where = "WHERE meme.date BETWEEN curdate() and curdate() + interval 24 hour";

    sendData(where, res);
});

app.get("/yesterday", (req, res) => {
    let where = "WHERE meme.date BETWEEN curdate() - interval 1 day and curdate()";

    sendData(where, res);
});

app.get("/last14days", (req, res) => {
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
