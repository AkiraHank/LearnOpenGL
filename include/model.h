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
  void Draw(Shader& shader);

private:
  /*  模型数据  */
  std::vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  std::vector<Mesh> meshes;
  std::string directory;
  /*  函数   */
  void loadModel(std::string path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
#endif