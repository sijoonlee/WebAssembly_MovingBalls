const express = require('express')
const app = express()


// setHeaders needed for old Express
// app.use(express.static('public'), {
//     setHeaders: (res, path, stat) => {
//         if (path.endsWith('.wasm')){
//             res.set('Content-Type', 'application/wasm')
//         }
//     }
// })

app.use(express.static('public'))

app.listen(8000, ()=>{
    console.log("server up!")
})