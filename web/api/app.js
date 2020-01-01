const express = require("express");
const cors = require("cors");
const mysql = require("mysql2");
const fs = require("fs");
const isnumber = require("is-number");

const dataScraper = require("./data-scraper");

const app = express();



const port = 5000;

let config = JSON.parse(fs.readFileSync("config.json"));

const pool = mysql.createPool({
    host: config.host,
    user: config.user,
    password: config.password,
    database: config.database,
    waitForConnections: true,
    connectionLimit: 10,
    queueLimit: 0
});

app.use(cors());
app.set("port", port);

app.get("/today", cors(), (req, res) => {
    let where = "WHERE meme.date BETWEEN curdate() and curdate() + interval 24 hour";

    sendData(where, res, req);
});

app.get("/yesterday", cors(), (req, res) => {
    let where = "WHERE meme.date BETWEEN curdate() - interval 1 day and curdate()";

    sendData(where, res, req);
});

app.get("/last14days", cors(), (req, res) => {
    let where = "WHERE meme.date BETWEEN curdate() - interval 14 day and curdate()";

    sendData(where, res, req);
});

app.get("/day/:day/month/:month/year/:year", (req, res) => {
    let day = req.params.day;
    let year = req.params.year;
    let month = req.params.month;

    if (!isnumber(day))
    {
        sendEmpty();
        return;
    }

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
    "${year}-${month}-${day}" and 
    "${year}-${month}-${day}" + interval 24 hour`;

    sendData(where, res, req);
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
    
    sendData(where, res, req);
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

    sendData(where, res, req);
});

app.get("/update/:id", (req, res) => {
    res.status(200).send("ok");
    
    let id = req.params.id;
    console.log(`Getting data for ${id}`);

    if (!isnumber(id))
    {
        console.warn("Not number");
        return;
    }

    let query = `SELECT url FROM meme WHERE id = ${id} AND dataType IS NULL`;

    pool.query(query, async (err, result) => {

        if (err) {
            throw err;
        }

        if (result.length === 0)
        {
            console.warn("Cannot find meme");
            return;
        }

        let url = result[0].url;

        let scrapedData = await dataScraper(url);

        if (scrapedData === null)
        {
            console.warn("Cannot get scrape data");
            return;
        }

        let query2 = `UPDATE meme SET dataType=${scrapedData.type},dataUrl='${scrapedData.url}' WHERE id=${id}`;

        pool.query(query2, (err2, result2) => {
            if (err2) {
                throw err2;
            }

            console.log(`Done ${id}`);
        });
    });
});

app.listen(port, () => {
    console.log(`Server running on port: ${port}`);
});

function parsePageQuery(req)
{
    let page = req.query.page;
    let limit = req.query.limit;
    
    if (page !== undefined && limit !== undefined)
    {
        if (!isnumber(page) || !isnumber(limit))
        {
            return null;
        }

        return { page: page, limit: limit };
    }
    else
    {
        return null;
    }
}

const SORTBY_KARMA = 1;
const SORTBY_DATE = 2;

function parseSort(req)
{
    let sortBy = parseInt(req.query.sortBy);
    let sortDesc = parseInt(req.query.sortDesc);

    if (sortBy === undefined || !isnumber(sortBy))
    {
        sortBy = SORTBY_KARMA;
    }

    if (sortDesc === undefined || !isnumber(sortDesc))
    {
        sortDesc = 1;
    }

    return { sortBy: sortBy, sortDesc: sortDesc };
}

async function countRows(dateFilter)
{
    let query = `SELECT COUNT(*) as count
    FROM meme
    JOIN author 
        ON author.id = meme.author_id
    ${dateFilter}`;

    const [ rows ] = await pool.promise().query(query);
                    
    return rows[0].count;
}

function sendData(dateFilter, res, req)
{
    let query = `SELECT meme.id, author.name, meme.url, meme.karma, meme.message, meme.date, meme.dataType, meme.dataUrl
    FROM meme 
    JOIN author 
        ON author.id = meme.author_id
    ${dateFilter}\n`;

    let pagination = null;
    let sortData = null;
    if (req !== null)
    {
        sortData = parseSort(req);

        if (sortData !== null)
        {
            switch (sortData.sortBy)
            {
                default:
                case SORTBY_KARMA:
                    query += "ORDER BY meme.karma";
                    break;

                case SORTBY_DATE:
                    query += "ORDER BY meme.date";
                    break;
            }

            if (sortData.sortDesc == 1)
            {
                query += " DESC";
            }

            query += '\n';
        }

        pagination = parsePageQuery(req);

        if (pagination !== null)
        {
            let offset = pagination.page * pagination.limit;

            query += `LIMIT ${pagination.limit} OFFSET ${offset}\n`;
        }
    }

    pool.query(query, async (err, result) => {
        if (err) {
            throw err;
        }
        let finalResult = {
            links: result
        };

        if (pagination !== null)
        {
            let rowCount = await countRows(dateFilter);
            
            finalResult.pagesCount = Math.ceil(rowCount / pagination.limit);
        }

        res.status(200).send(finalResult);
    });
}

function sendEmpty()
{
    res.status(200).send({});
}
