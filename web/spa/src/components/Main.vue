<template>
    <main class="main-container">
        <h1>Najlepsze memy <a href="https://poorchat.net/channels/jadisco">#jadisco</a></h1>

        <div id="left-side">
            <b-radio-group id="radio-group-2 radio" v-model="selectedFilter" @input="onRadioChange()" buttons>
                <b-radio :value=1>Dzisiaj</b-radio>
                <b-radio :value=2>Wczoraj</b-radio>
                <b-radio :value=3>Ostatnie 14 dni</b-radio>
                <b-radio :value=4>Wybierz</b-radio>
            </b-radio-group>

            <div v-if="selectedFilter == 4">
                <b-input @change="onDateChange()" v-model="customDate" type="date" class="date-picker" />
            </div>
        </div>

        <div id="right-side">
            <b-dropdown id="dropdown" :text="selectedSortingElement.text" >
                <b-dropdown-item v-for="item in sortings" :key="item.id" @click="onDropDownClick(item)">{{item.text}}</b-dropdown-item>
            </b-dropdown>
        </div>

        <div class="clear"></div>

        <div v-if="pagesCount > 1">
            <b-pagination
                id="pagination"
                v-model="currentPage"
                :total-rows="pagesCount * 20"
                :per-page="20"
                first-text="<<"
                prev-text="<"
                next-text=">"
                last-text=">>"
                align="fill"
                @change="onPageChange"
            ></b-pagination>
        </div>

        <!------------------------------------------------------------------------------------------->

        <img id="loading" v-if="loading" src="loading.svg">

        <div class="items">
            <Link v-for="link in items" :key="link.id" :data="link" />
        </div>

        <!------------------------------------------------------------------------------------------->

        <div v-if="pagesCount > 1">
            <b-pagination
                id="pagination"
                v-model="currentPage"
                :total-rows="pagesCount * 20"
                :per-page="20"
                first-text="<<"
                prev-text="<"
                next-text=">"
                last-text=">>"
                align="fill"
                @change="onPageChange"
            ></b-pagination>
        </div>

        <div id="left-side">
            <b-radio-group id="radio-group-2 radio" v-model="selectedFilter" @input="onRadioChange()" buttons>
                <b-radio :value=1>Dzisiaj</b-radio>
                <b-radio :value=2>Wczoraj</b-radio>
                <b-radio :value=3>Ostatnie 14 dni</b-radio>
                <b-radio :value=4>Wybierz</b-radio>
            </b-radio-group>

            <div v-if="selectedFilter == 4">
                <b-input @change="onDateChange()" v-model="customDate" type="date" class="date-picker" />
            </div>
        </div>

        <div id="right-side">
            <b-dropdown id="dropdown" :text="selectedSortingElement.text" >
                <b-dropdown-item v-for="item in sortings" :key="item.id" @click="onDropDownClick(item)">{{item.text}}</b-dropdown-item>
            </b-dropdown>
        </div>

        <div class="clear"></div>
    </main>
</template>

<script>
import MemeApi, { ApiQuery, SortBy } from "../api/MemeApi";
import Link from "./Link";
import dateformat from "dateformat";

/* eslint no-console: ["error", { allow: ["info", "log"] }] */

export default {
    name: 'Main',
    components: { Link },
    created() {
        this.selectedSortingElement = this.sortings[0];
    },
    async mounted() {
        await this.loadList();
        this.loading = false;
    },
    data() {
        return {
            loading: true,
            selectedFilter: 1,
            lastSelectedFilter: 0,
            customDate: 0,
            sortings: [
                { id: 0, text: "Malejąco według karmy" },
                { id: 1, text: "Rosnąco według karmy" },
                { id: 2, text: "Malejąco według daty" },
                { id: 3, text: "Rosnąco według daty" }
            ],
            selectedSortingElement: null,
            apiQuery: new ApiQuery(0, SortBy.KARMA, 1),
            pagesCount: 0,
            currentPage: 1,
            items: []
        };
    },
    methods: {
        dateformat: dateformat,
        async requestApi() {
            switch (this.selectedFilter)
            {
                case 1:
                    return await MemeApi.today(this.apiQuery);
                case 2:
                    return await MemeApi.yesterday(this.apiQuery);
                case 3:
                    return await MemeApi.last14days(this.apiQuery);
                case 4:
                    var date = new Date(this.customDate);
                    return await MemeApi.getDay(date.getDate(), date.getMonth() + 1, date.getFullYear(), this.apiQuery);
            }
        },
        async loadList() {
            console.log("asdf");
            let sortingId = localStorage.getItem("sorting");

            if (sortingId !== null)
            {
                sortingId = parseInt(sortingId);
            }
            else
            {
                sortingId = 0;
            }
            
            this.selectedSortingElement = this.sortings[sortingId];

            switch (sortingId) {
                case 0: // by karma desc
                    this.apiQuery.sortBy = SortBy.KARMA;
                    this.apiQuery.sortDesc = 1;
                    break;
                case 1: // by karma asc
                    this.apiQuery.sortBy = SortBy.KARMA;
                    this.apiQuery.sortDesc = 0;
                    break;
                case 2: // by date desc
                    this.apiQuery.sortBy = SortBy.DATE;
                    this.apiQuery.sortDesc = 1;
                    break;
                case 3: // by date asc
                    this.apiQuery.sortBy = SortBy.DATE;
                    this.apiQuery.sortDesc = 0;
                    break;
                
                default:
                    break;
            }
            
            this.apiQuery.page = this.currentPage - 1;

            let list = await this.requestApi();
            
            let listData = list.data;
            this.items = listData.links;
            this.pagesCount = listData.pagesCount;
        },
        async onRadioChange() {

            // prevent double calling
            if (this.selectedFilter == this.lastSelectedFilter)
            {
                return;
            }

            this.lastSelectedFilter = this.selectedFilter;

            this.loading = true;

            if (this.selectedFilter !== 4)
            {
                this.items = [];
            }

            await this.loadList();

            this.loading = false;
        },
        async onDateChange() {
            this.loadList();
        },
        onDropDownClick(item) {
            this.selectedSortingElement = item;

            localStorage.setItem("sorting", item.id);

            this.loadList();
        },
        onPageChange(pageNum) {
            this.currentPage = pageNum;
            this.loadList();
        }
    }
}
</script>


<style>
/* i don't know how to do it in the right way xd */
.page-item .page-link {  
    background-color: #6c757d !important;  
    border-color: #6c757d !important;  
    color: white;
}

.page-item:hover .page-link {  
    background-color: #5a6268 !important;  
    border-color: #5a6268 !important;  
    color: white;
    transition-duration: 0.25s;
}

.page-item.active .page-link {  
    background-color: #545b62 !important;  
    border-color: #545b62 !important;  
}

.page-item.disabled .page-link {  
    color:rgb(214, 214, 214) !important;
}
</style>

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

#loading {
    display: block;
    margin: 0 auto;
}

#pagination {
    margin: 40px 0;
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
