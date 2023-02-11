#ifndef MODEL_H
#define MODEL_H

#include "myShader.h"
#include "mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model {
public:
  /*  函数   */
  Model(char* path) {
    loadModel(path);
  }
  void Draw(Shader shader);

private:
  /*  模型数据  */
  std::vector<Mesh> meshes;
  std::string directory;
  /*  函数   */
  void loadModel(string path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
#endif