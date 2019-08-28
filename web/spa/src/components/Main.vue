<template>
    <main class="main-container">
        <h1>Najlepsze memy</h1>

        <b-radio-group id="radio-group-2" v-model="selected" @input="onRadioChange()" buttons>
            <b-radio :value=1>Dzisiaj</b-radio>
            <b-radio :value=2>Wczoraj</b-radio>
            <b-radio :value=3>Ostatnie 14 dni</b-radio>
            <b-radio :value=4>Konkretny dzień</b-radio>
        </b-radio-group>

        <div v-if="selected == 4">
            <b-input @change="onDateChange()" v-model="customDate" type="date" class="date-picker" />
        </div>

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
    async mounted() {
        let data = await MemeApi.today();
        this.loadList(data.data);
    },
    data() {
        return {
            selected: 1,
            customDate: 0,
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
    border-top: 1px solid #aaa;
}

@media (max-width: 1000px) {
    .main-container {
        width: 90%;
    }

    
}
</style>
