import Api from "./base";

export default {
  async today() {
    return await Api().get("/today");
  },
  async yesterday() {
    return await Api().get("/yesterday");
  },
  async last14days() {
    return await Api().get("/last14days");
  },
  async get(year) {
    return await Api().get(`/year/${year}`);
  },
  async get(month, year) {
    return await Api().get(`/month/${month}/year/${year}`);
  }
};
