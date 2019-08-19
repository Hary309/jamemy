<template>
  <div>
    <Main />
    <Footer />
  </div>
</template>

<script>
import Main from './components/Main';
import Footer from './components/Footer';

export default {
  name: 'app',
  components: {
     Main, Footer
  },
  methods: {

  }
}
</script>

<style src="bootstrap/dist/css/bootstrap.min.css">
</style>
