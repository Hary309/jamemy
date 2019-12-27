import Api from "./base";

export const SortBy = {
  KARMA: 1,
  DATE: 2
};

export class ApiQuery
{
  static limit = 20;

  constructor(page, sortBy, sortDesc)
  {
    this.page = page;
    this.sortBy = sortBy;
    this.sortDesc = sortDesc;
  }
}

export default {
  async sendRequest(url, apiQuery) {
    return await Api().get(url + `?limit=${ApiQuery.limit}&page=${apiQuery.page}&sortBy=${apiQuery.sortBy}&sortDesc=${apiQuery.sortDesc}`);
  },
  async today(apiQuery) {
    return await this.sendRequest("/today", apiQuery);
  },
  async yesterday(apiQuery) {
    return await this.sendRequest("/yesterday", apiQuery);
  },
  async last14days(apiQuery) {
    return await this.sendRequest("/last14days", apiQuery);
  },
  async getYear(year, apiQuery) {
    return await this.sendRequest(`/year/${year}`, apiQuery);
  },
  async getMonth(month, year, apiQuery) {
    return await this.sendRequest(`/month/${month}/year/${year}`, apiQuery);
  },
  async getDay(day, month, year, apiQuery) {
    return await this.sendRequest(`/day/${day}/month/${month}/year/${year}`, apiQuery);
  }
};
