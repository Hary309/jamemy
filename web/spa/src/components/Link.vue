<template>
    <div class="link">
        <div class="link-header">
            <div class="link-author">{{data.name}}</div>
            <div class="link-date">{{dateformat(data.date, "HH:MM:ss dd-mm-yyyy")}}</div>
            <div class="link-karma">+{{data.karma}}</div>
        </div>

        <div class="clear"></div>
        
        <div class="link-message" v-if="data.message.length != 0">
            <b>Msg:</b> {{data.message}}
        </div>
        <img class="image" v-if="data.dataType == 1" :src="data.dataUrl" alt="">
        <video class="video" v-if="data.dataType == 2" :src="data.dataUrl" autoplay controls loop muted></video>
        <div class="embed" v-if="data.dataType == 3">
            <iframe :src="data.dataUrl" width="1920" height="1080"></iframe>
        </div>
        <div class="link-src">
            <span>Src: </span>
            <a :href="data.url" target="_blank">{{getHostname(data.url)}}</a>
        </div>
    </div>
</template>

<script>
import dateformat from "dateformat";
import urlParse from "url-parse";

export default {
    name: "Link",
    props: {
        data: {
            id: Number,
            name: String, // author
            url: String,
            karma: Number,
            date: Date,
            message: String,
            dataType: Number,
            dataUrl: String
        }
    },
    methods: {
        dateformat: dateformat,
        getHostname(url) {
            let parsedUrl = urlParse(url);

            return parsedUrl.hostname;
        }
    }
}
</script>

<style scoped>
.image {
    max-width: 100%;
    height: auto;
    margin: 0 auto;
}

.video {
    max-width: 100%;
    height: auto;
}

.embed {
    padding-top: 56.25%;
    position:relative;
}

.embed iframe {
    position: absolute;
    top:0;
    left:0;
    width:100%;
    height:100%;
    border: 0;
}

.link {
    color: white;
    margin: 32px auto;
    padding: 16px;
    font-family: Arial, Helvetica, sans-serif;
    font-size: 18px;
    border: 1px solid #222222;
    border-radius: 4px;
    background: #222222;
}


.link-author, .link-date, .link-karma {
    margin-bottom: 4px;
}

.link-author {
    float: left;
    font-weight: bold;
}

.link-date {
    float: left;
    margin-left: 8px;
}

.link-karma {
    float: right;
    color: #4699ff;
}

.link-message {
    border-top: 1px solid #333;
    padding-top: 8px;
    margin-bottom: 8px;
    overflow: hidden;
    text-overflow: ellipsis;
}

.link-src {
    margin-top: 8px;
    max-width: 80%;
    font-size: 12px;
}

.link-src a {
    display: inline;
    max-width: 80%;
}
</style>