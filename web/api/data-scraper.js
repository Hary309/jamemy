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
/// 3 - embedded
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

    if (url.includes("youtube.com") || url.includes("youtu.be"))
    {
        let parsedUrl = urlParser(url, true);

        return {
            type: 3,
            url: `https://www.youtube.com/embed/${parsedUrl.query.v}`
        }
    }

    const metadata = await metascraper({ html, url });

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
        if (url.includes("streamable"))
        {
            let videoUrl = urlParser(url);
            let returnUrl = videoUrl.origin + "/o" + videoUrl.pathname;

            return {
                type: 3,
                url: returnUrl
            }
        }

        return {
            type: 2,
            url: metadata.video
        }
    }

    return null;
}
