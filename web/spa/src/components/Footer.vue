<template>
    <footer class="container">
        <div class="copyright">
            <p>Piotr Krupa &copy; 2019</p>
        </div>
        <div class="links">
            <a href="https://github.com/Harry09/jamemy">Github</a>
        </div>

        <div class="clear"></div>
    </footer>
</template>

<script>
export default {
  name: 'Footer'
}
</script>

<style>
.container {
    padding: 36px;
}

.copyright {
    color: #AAA;
    float: left;
}

.links {
    float: right;
}

.clear {
    clear: both;
}
</style>