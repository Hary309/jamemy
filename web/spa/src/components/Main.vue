<template>
    <main role="main" class="container">
        <h1>Najlepsze memy</h1>

        <b-dropdown v-bind:text="selected" variant="primary">
            <b-dropdown-item v-for="type in types" :key="type.text" @click="dropDownClick(type)">{{type.text}}</b-dropdown-item>
        </b-dropdown>

        <b-table class="table" striped hover :items="list.items" :fields="list.fields">
            <template slot="[url]" slot-scope="data">
                <a :href="data.item.url.full" target="_blank">{{data.item.url.short}}</a>
            </template>

            <template slot="[date]" slot-scope="data">
                {{dateformat(data.item.date, "HH:MM:ss dd-mm-yyyy")}}
            </template>
        </b-table>
    </main>
</template>

<script>
import MemeApi from "../api/MemeApi";
import dateformat from "dateformat";
import cropUrl from "crop-url";

export default {
    name: 'Main',
    async mounted() {
        let data = await MemeApi.today();
        this.loadList(data.data);
    },
    data() {
        return {
            selected: "Dzisiaj",
            types: [
                { text: "Dzisiaj", method: MemeApi.today },
                { text: "Wczoraj", method: MemeApi.yesterday },
                { text: "Ostatnie 14 dni", method: MemeApi.last14days }
            ],
            list: {
                fields:{
                    name: {
                        label: "Autor"
                    },
                    url: {
                        label: "Url"
                    },
                    karma: {
                        label: "Karma",
                        sortable: true
                    },
                    date: {
                        label: "Data dodania",
                        sortable: true
                    }
                },
                items: []
            }
        };
    },
    methods: {
        dateformat: dateformat,
        loadList(list) {
            list.forEach(element => {
                let url = element.url;
                element.url = {
                    full: url,
                    short: cropUrl(url, 50)
                };
            });

            this.list.items = list;
        },
        async dropDownClick(item) {
            this.selected = item.text;

            let data = await item.method();

            this.loadList(data.data);
        }
    }
}
</script>

<style scoped>
.container {
    margin-top: 36px;
}

.table {
    margin-top: 16px;
}
</style>
