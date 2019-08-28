const metascraper = require("metascraper")([
    require("metascraper-image")(),
    require("metascraper-video")(),
    ]);
const got = require('got');

const urlParser = require("url-parse");

module.exports = scrapData;



/// Type:
/// 1 - image
/// 2 - video

async function scrapData(targetUrl) {
    const { headers, body: html, url } = await got(targetUrl)

    const contentType = headers["content-type"];

    if (contentType.includes("image"))
    {
        return {
            type: 1,
            url: targetUrl
        }
    }
    else if (contentType.includes("video"))
    {
        return {
            type: 2,
            url: targetUrl
        }
    }

    const metadata = await metascraper({ html, url })

    if (metadata.image !== null && metadata.video === null)
    {
        let imageUrl = urlParser(metadata.image);
        let returnUrl = metadata.image;
        
        if (imageUrl.hostname.includes("imgur"))
        {
            returnUrl = returnUrl.replace("fb", "");
        }

        return {
            type: 1,
            url: returnUrl
        }
    }
    else if (metadata.video !== null)
    {
        return {
            type: 2,
            url: metadata.video
        }
    }

    return null;
}
