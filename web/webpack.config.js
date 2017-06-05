var path = require('path');
var ExtractTextPlugin = require('extract-text-webpack-plugin');

const config = {
    devServer: {
        inline: true,
        contentBase: './',
        port: 9643
    },
    entry: './src/app.js',
    devtool: 'source-map',
    output: {
        path: path.resolve(__dirname, 'dist'),
        filename: 'bundle.js',
        publicPath: '/dist'
    },
    module: {
        rules: [
            {
                test: /\.js$/,
                loaders: ['babel-loader'],
                exclude: /node_modules/
            },
            {
                test: /\.scss$/,
                loader: ExtractTextPlugin.extract(['css-loader', 'sass-loader'])
            }
        ]
    },
    plugins: [
        new ExtractTextPlugin({
            filename: '[name].bundle.css',
            allChunks: true,
        })
    ]
};

module.exports = config;
