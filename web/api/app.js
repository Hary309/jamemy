const express = require("express");
const cors = require("cors");
const mysql = require("mysql");
const fs = require("fs");
const isnumber = require("is-number");

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
    let year = req.params.year;
    let month = req.params.month;

    if (!isnumber(year))
    {
        sendEmpty();
        return;
    }

    if (!isnumber(month))
    {
        sendEmpty();
        return;
    }

    let where = `
    WHERE meme.date BETWEEN
    "${year}-${month}-01" and 
    "${year}-${month}-01" + interval 1 month - interval 1 second`;

    sendData(where, res);
});

app.get("/year/:year", (req, res) => {
    let year = req.params.year;

    if (!isnumber(year))
    {
        sendEmpty();
        return;
    }

    let where = `
    WHERE meme.date BETWEEN
    "${year}-01-01" and 
    "${year}-01-01" + interval 12 month - interval 1 second`;

    sendData(where, res);
});

app.listen(port, () => {
    console.log(`Server running on port: ${port}`);
});

function sendData(dateFilter, res)
{
    let query = `SELECT meme.id, author.name, meme.url, meme.karma, meme.date
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

function sendEmpty()
{
    res.status(200).send({});
}
