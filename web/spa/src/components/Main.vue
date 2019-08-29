<template>
    <main class="main-container">
        <h1>Najlepsze memy</h1>

        <div id="left-side">
            <b-radio-group id="radio-group-2 radio" v-model="selected" @input="onRadioChange()" buttons>
                <b-radio :value=1>Dzisiaj</b-radio>
                <b-radio :value=2>Wczoraj</b-radio>
                <b-radio :value=3>Ostatnie 14 dni</b-radio>
                <b-radio :value=4>Wybierz</b-radio>
            </b-radio-group>

            <div v-if="selected == 4">
                <b-input @change="onDateChange()" v-model="customDate" type="date" class="date-picker" />
            </div>
        </div>

        <div id="right-side">
            <b-dropdown id="dropdown" :text="selectedSorting.text" >
                <b-dropdown-item v-for="item in sortings" :key="item.id" @click="onDropDownClick(item)">{{item.text}}</b-dropdown-item>
            </b-dropdown>
        </div>

        <div class="clear"></div>

        <div class="items">
            <Link v-for="link in items" :key="link.id" :data="link" />
        </div>
    </main>
</template>

<script>
import MemeApi from "../api/MemeApi";
import Link from "./Link";
import dateformat from "dateformat";

export default {
    name: 'Main',
    components: { Link },
    created() {
        this.selectedSorting = this.sortings[1];
    },
    async mounted() {
        let data = await MemeApi.today();
        this.loadList(data.data);
    },
    data() {
        return {
            selected: 1,
            customDate: 0,
            sortings: [
                { id: 0, text: "Malejąco według karmy" },
                { id: 1, text: "Rosnąco według karmy" },
                { id: 2, text: "Malejąco według daty" },
                { id: 3, text: "Rosnąco według daty" }
            ],
            selectedSorting: null,
            items: []
        };
    },
    methods: {
        dateformat: dateformat,
        loadList(list) {
            this.items = list;
        },
        async onRadioChange() {
            let data = null;

            switch (this.selected)
            {
                case 1:
                    data = await MemeApi.today();
                    break;
                case 2:
                    data = await MemeApi.yesterday();
                    break;
                case 3:
                    data = await MemeApi.last14days();
                    break;
                case 4:
                    break;
            }

            if (data !== null)
            {
                this.loadList(data.data);
            }
        },
        async onDateChange() {
            var date = new Date(this.customDate);

            let data = await MemeApi.getDay(date.getDate(), date.getMonth() + 1, date.getFullYear());

            if (data !== null)
            {
                this.loadList(data.data);
            }
        },
        async onDropDownClick(item) {
            this.selectedSorting = item;

            switch (item.id)
            {
                case 0:
                    this.items.sort((a, b) => { console.log(a); return a.karma - b.karma });
                    break;
                case 1:
                    this.items.sort((a, b) => { return b.karma - a.karma });
                    break;
                case 2:
                    this.items.sort((a, b) => { console.log(a); return new Date(a.date) - new Date(b.date) });
                    break;
                case 3:
                    this.items.sort((a, b) => { console.log(a); return new Date(b.date) - new Date(a.date) });
                    break;
            }
        }
    }
}
</script>

<style scoped>
.main-container {
    width: 900px;
    margin: 0 auto;
}

h1 {
    color: white;
    margin: 32px 0;
}

.change-button {
    margin-right: 4px;
}

.date-picker {
    margin-top: 4px;
    width: 200px;
}

.items {
    margin-top: 16px;
    padding-top: 16px;
}

#left-side {
    float: left;
}

#left-side input {
    margin: 0 auto;
    margin-top: 8px;
}

#right-side {
    float: right;
}

@media (max-width: 1000px) {
    .main-container {
        width: 90%;
    }
}

@media (max-width: 640px) {
    #left-side {
        float: none;
        display: inline-block;
        width: 100%;
        text-align: center;
        margin-bottom: 8px;
    }

    #right-side {
        display: inline-block;
        width: 100%;
        float: none;
        margin: 0 auto;
        text-align: center;
    }
}
</style>
