<template>
    <footer class="container">
        <div class="left">
            <p>Celem strony jest zbieranie najlepszych memów z czatu Poorchat z kanału #jadisco</p>
        </div>
        <div class="right">
            Pytania, problemy? Pisz na
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
    color: #AAA;
    padding: 36px;
}

.left {
    float: left;
}

.right {
    float: right;
}
</style>