const express = require('express');
const app = express();
const path = require('path');
const port = 3003;

// Servir archivos estáticos desde la carpeta 'FrontEnd'
app.use(express.static(path.join(__dirname, 'C:/Users/ESBEIDY/Documents/IHC_4.2/FrontEnd')));

// Array para almacenar los valores
let valoresGuardados = [];

// Ruta para guardar los valores enviados desde Thunder Client
app.get('/api/guardar', (req, res) => {
  const { x, y, z } = req.query;

  // Validar y asegurar que los valores se reciban correctamente
  if (x !== undefined && y !== undefined && z !== undefined) {
    const valor = {
      x: parseFloat(x),
      y: parseFloat(y),
      z: parseFloat(z)
    };
    // Guardar los valores recibidos
    valoresGuardados.push(valor);
    return res.status(200).send('Valores guardados exitosamente');
  }

  return res.status(400).send('Todos los valores (x, y, z) son necesarios.');
});

// Ruta para devolver los valores guardados
app.get('/api/valores', (req, res) => {
  res.json(valoresGuardados);
});

// Ruta para servir la página HTML
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'C:/Users/ESBEIDY/Documents/IHC_4.2/FrontEnd', 'index.html'));
});

// Iniciar el servidor
app.listen(port, () => {
  console.log(`Servidor ejecutándose en http://localhost:${port}`);
});
