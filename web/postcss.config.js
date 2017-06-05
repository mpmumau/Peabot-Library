module.exports = {
  parser: 'postcss-sass',
  plugins: {
    'postcss-import': {},
    'cssnext': {},
    'autoprefixer': require('autoprefixer')(),
    'cssnano': {}
  }
}