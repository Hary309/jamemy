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

        <div class="link-src">
            <span>Src: </span>
            <a :href="data.url" target="_blank">{{getHostname(data.url)}}</a>
        </div>
    </div>
</template>

<script>
import dateformat from "dateformat";
import utf8 from "utf8";
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
    mounted() {
        // this.data.message = utf8.decode(this.data.message);
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

.clear {
	clear: both;
}
</style>