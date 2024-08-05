// #include "Mesh.h"

// #include "Rndr/Core/Log.h"

// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
// #include <assimp/Importer.hpp>
// #include <assimp/DefaultLogger.hpp>
// #include <assimp/LogStream.hpp>

// #include <glad/glad.h>

// namespace Rndr {

// 	namespace {
// 		const unsigned int ImportFlags =
// 			aiProcess_CalcTangentSpace |
// 			aiProcess_Triangulate |
// 			aiProcess_SortByPType |
// 			aiProcess_PreTransformVertices |
// 			aiProcess_GenNormals |
// 			aiProcess_GenUVCoords |
// 			aiProcess_OptimizeMeshes |
// 			aiProcess_Debone |
// 			aiProcess_ValidateDataStructure;
// 	}

// 	struct LogStream : public Assimp::LogStream
// 	{
// 		static void Initialize()
// 		{
// 			if (Assimp::DefaultLogger::isNullLogger())
// 			{
// 				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
// 				Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
// 			}
// 		}

// 		void write(const char* message) override
// 		{
// 			RNDR_CORE_ERROR("Assimp error: {0}", message);
// 		}
// 	};

// 	Mesh::Mesh(const std::string& filename)
// 		: m_FilePath(filename)
// 	{
// 		LogStream::Initialize();

// 		RNDR_CORE_INFO("Loading mesh: {0}", filename.c_str());

// 		Assimp::Importer importer;
		
// 		const aiScene* scene = importer.ReadFile(filename, ImportFlags);
// 		if (!scene || !scene->HasMeshes())
// 			RNDR_CORE_ERROR("Failed to load mesh file: {0}", filename);

// 		aiMesh* mesh = scene->mMeshes[0];

// 		RNDR_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
// 		RNDR_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

// 		m_VertexArray = VertexArray::Create();

// 		m_Vertices.reserve(mesh->mNumVertices);

// 		// Extract vertices from model
// 		for (size_t i = 0; i < m_Vertices.capacity(); i++) 
// 		{
// 			Vertex vertex;
// 			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
// 			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

// 			// if (mesh->HasTangentsAndBitangents())
// 			// {
// 			// 	vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
// 			// 	vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
// 			// }

// 			// if (mesh->HasTextureCoords(0))
// 			// 	vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
// 			m_Vertices.push_back(vertex);
// 		}

// 		// m_VertexBuffer.reset(VertexBuffer::Create());
// 		m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
// 		BufferLayout layout = {
// 			{ ShaderDataType::Float3, "a_Position" },
// 			{ ShaderDataType::Float3, "a_Normal" },
// 			{ ShaderDataType::Float3, "a_Tangent" },
// 			{ ShaderDataType::Float3, "a_Binormal" },
// 			{ ShaderDataType::Float2, "a_Texcoord" },
// 			{ ShaderDataType::Int, "a_EntityID" }
// 		};
// 		m_VertexBuffer->SetLayout(layout);
// 		// m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
// 		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

// 		// Extract indices from model 
// 		m_Indices.reserve(mesh->mNumFaces);
// 		for (size_t i = 0; i < m_Indices.capacity(); i++)
// 		{
// 			RNDR_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
// 			m_Indices.push_back({ mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] });
// 		}

// 		// m_IndexBuffer.reset(IndexBuffer::Create());
// 		m_IndexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Indices.size() * sizeof(Index));
// 		// m_IndexBuffer->SetData(m_Indices.data(), m_Indices.size() * sizeof(Index));
// 		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

// 		RNDR_CORE_INFO("Loaded mesh: {0}", filename);
// 	}

// 	Mesh::~Mesh()
// 	{
// 	}

// 	void Mesh::Render()
// 	{

// 	}

// }



#include "Mesh.h"

#include "Rndr/Core/Log.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <glad/glad.h>


// namespace Rndr {

// 	namespace {
// 		const unsigned int ImportFlags =
// 			aiProcess_Triangulate |
// 			aiProcess_SortByPType |
// 			aiProcess_PreTransformVertices |
// 			aiProcess_GenNormals |
// 			aiProcess_GenUVCoords |
// 			aiProcess_OptimizeMeshes |
// 			aiProcess_Debone |
// 			aiProcess_ValidateDataStructure;
// 	}

// 	struct LogStream : public Assimp::LogStream
// 	{
// 		static void Initialize()
// 		{
// 			if (Assimp::DefaultLogger::isNullLogger())
// 			{
// 				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
// 				Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
// 			}
// 		}

// 		void write(const char* message) override
// 		{
// 			RNDR_CORE_ERROR("Assimp error: {0}", message);
// 		}
// 	};

// 	Mesh::Mesh(const std::string& filename)
// 		: m_FilePath(filename)
// 	{
// 		LogStream::Initialize();

// 		RNDR_CORE_INFO("Loading mesh: {0}", filename.c_str());

// 		Assimp::Importer importer;
		
// 		const aiScene* scene = importer.ReadFile(filename, ImportFlags);
// 		if (!scene || !scene->HasMeshes())
// 		{
// 			RNDR_CORE_ERROR("Failed to load mesh file: {0}", filename);
// 			RNDR_CORE_ASSERT(false, "Failed to load mesh file.");
// 		}


// 		uint32_t numVertices = 0;
// 		uint32_t numIndices = 0;
// 		uint32_t numFaces = 0;
// 		for (int i = 0; i < scene->mNumMeshes; i++)
// 		{
// 			numVertices += scene->mMeshes[i]->mNumVertices;
// 			numFaces += scene->mMeshes[i]->mNumFaces;
// 		}
// 		numIndices = numFaces * 3;

// 		m_Vertices.reserve(numVertices);
// 		m_Triangles.reserve(numFaces);
// 		m_Indices.reserve(numIndices);

// 		uint32_t vertexOffset = 0;


// 		m_SubMeshes.reserve(scene->mNumMeshes);
// 		for (int currentMesh = 0; currentMesh < scene->mNumMeshes; currentMesh++)
// 		{
// 			aiMesh* mesh = scene->mMeshes[currentMesh];
// 			RNDR_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
// 			RNDR_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

// 			SubMesh& subMesh = m_SubMeshes.emplace_back();

// 			// Vertices
// 			for (size_t i = 0; i < mesh->mNumVertices; i++) 
// 			{
// 				Vertex vertex;
// 				vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
// 				vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

// 				if (mesh->HasTextureCoords(0))
// 					vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

// 				m_Vertices.push_back(vertex);

// 				m_VerticesPositions.push_back({vertex.Position, 1.0f});
// 				m_VerticesNormals.push_back({vertex.Normal, 1.0f});
// 			}

// 			// Indices
// 			for (size_t i = 0; i < mesh->mNumFaces; i++)
// 			{
// 				RNDR_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
// 				Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
// 				index.V1 += vertexOffset;
// 				index.V2 += vertexOffset;
// 				index.V3 += vertexOffset;

// 				Triangle triangle;
// 				triangle.V1 = m_VerticesPositions[index.V1];
// 				triangle.V2 = m_VerticesPositions[index.V2];
// 				triangle.V3 = m_VerticesPositions[index.V3];
// 				m_Triangles.push_back(triangle);

// 				m_VerticesIndex.push_back(index.V1);
// 				m_VerticesIndex.push_back(index.V2);
// 				m_VerticesIndex.push_back(index.V3);

// 				m_Indices.push_back(index);
// 			}
// 			vertexOffset += mesh->mNumVertices;
// 		}

// 		m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
// 		BufferLayout layout = {
// 			{ ShaderDataType::Float3, "a_Position" },
// 			{ ShaderDataType::Float3, "a_Normal" },
// 			{ ShaderDataType::Float2, "a_Texcoord" },
// 			{ ShaderDataType::Int, "a_EntityID" }
// 		};
// 		m_VertexBuffer->SetLayout(layout);

// 		m_VertexArray = VertexArray::Create();
// 		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


// 		m_IndexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Indices.size() * sizeof(uint32_t));
// 		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

// 		// RNDR_CORE_INFO("Loaded mesh: {0}", filename);


// 		int vertSize = m_Vertices.size();
// 		int triSize = m_Triangles.size();
// 		int indSize = m_Indices.size();
// 		int posSize = m_VerticesPositions.size();
// 		int norSize = m_VerticesNormals.size();
// 		int indPosSize = m_VerticesIndex.size();

// 		RNDR_CORE_INFO("Vertices: {0}", vertSize);
// 		RNDR_CORE_INFO("Triangles: {0}", triSize);
// 		RNDR_CORE_INFO("Indices: {0}", indSize);
// 		RNDR_CORE_INFO("Positions: {0}", posSize);
// 		RNDR_CORE_INFO("Normals: {0}", norSize);
// 		RNDR_CORE_INFO("Index Positions: {0}", indPosSize);

// 		// exit(0);
// 	}


// 	Mesh::~Mesh()
// 	{
// 	}

// 	void Mesh::Render()
// 	{

// 	}

// }

#include "Rndr/Scene/Entity.h"

namespace Rndr {

	namespace {
		const unsigned int ImportFlags =
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_PreTransformVertices |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_Debone |
			aiProcess_ValidateDataStructure;
	}

	struct LogStream : public Assimp::LogStream
	{
		static void Initialize()
		{
			if (Assimp::DefaultLogger::isNullLogger())
			{
				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
				Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
			}
		}

		void write(const char* message) override
		{
			RNDR_CORE_ERROR("Assimp error: {0}", message);
		}
	};


	static void PrintAllNodes(aiNode* node, int depth = 0) {
		if (!node) return;

		// Print the current node name with indentation based on depth
		std::string sp;
		for (int i = 0; i < depth; ++i) {
			// std::cout << "  "; // Indent two spaces per level
			sp += "  ";
		}
		// std::cout << node->mName.C_Str() << std::endl;
		std::string name = sp + node->mName.C_Str();
		RNDR_CORE_INFO("{0}", name.c_str());

		// Check the number of children and iterate through them
		unsigned int numChildren = node->mNumChildren;
		// std::cout << std::string(depth * 2, ' ') << "Children count: " << numChildren << std::endl;

		for (unsigned int i = 0; i < numChildren; ++i) {
			aiNode* childNode = node->mChildren[i];
			if (childNode) {
				PrintAllNodes(childNode, depth + 1);
			} else {
				// std::cout << std::string((depth + 1) * 2, ' ') << "Null child at index " << i << std::endl;
			}
		}
	}

	Mesh::Mesh(const std::string& filename)
		: m_FilePath(filename)
	{
		LogStream::Initialize();

		RNDR_CORE_INFO("Loading mesh: {0}", filename.c_str());

		Assimp::Importer importer;
		
		const aiScene* scene = importer.ReadFile(filename, ImportFlags);
		if (!scene || !scene->HasMeshes())
		{
			RNDR_CORE_ERROR("Failed to load mesh file: {0}", filename);
			RNDR_CORE_ASSERT(false, "Failed to load mesh file.");
		}


		uint32_t numVertices = 0;
		uint32_t numIndices = 0;
		uint32_t numFaces = 0;
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			numVertices += scene->mMeshes[i]->mNumVertices;
			numFaces += scene->mMeshes[i]->mNumFaces;
		}
		numIndices = numFaces * 3;

		m_Vertices.reserve(numVertices);
		m_Triangles.reserve(numFaces);
		m_Indices.reserve(numIndices);



		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		m_SubMeshes.reserve(scene->mNumMeshes);
		int children = scene->mRootNode->mNumChildren;
		// RNDR_CORE_INFO("Children: {0}", children);
		RNDR_CORE_INFO("Number of meshes: {0}", scene->mNumMeshes);
		// PrintAllNodes(scene->mRootNode);
		// exit(0);
		for (int currentMesh = 0; currentMesh < scene->mNumMeshes; currentMesh++)
		{
			aiMesh* mesh = scene->mMeshes[currentMesh];
			RNDR_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
			RNDR_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");


			SubMesh& subMesh = m_SubMeshes.emplace_back();
			subMesh.BaseVertex = vertexCount;
			subMesh.BaseIndex = indexCount;
			subMesh.MaterialIndex = mesh->mMaterialIndex;
			subMesh.IndexCount = mesh->mNumFaces * 3;

			vertexCount += mesh->mNumVertices;
			indexCount += mesh->mNumFaces * 3;


			// Vertices
			for (size_t i = 0; i < mesh->mNumVertices; i++) 
			{
				Vertex vertex;
				vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
				vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

				if (mesh->HasTextureCoords(0))
					vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

				m_Vertices.push_back(vertex);

				m_VerticesPositions.push_back({vertex.Position, 1.0f});
				m_VerticesNormals.push_back({vertex.Normal, 1.0f});
			}

			// Indices
			for (size_t i = 0; i < mesh->mNumFaces; i++)
			{
				RNDR_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
				Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
				// index.V1 += vertexOffset;
				// index.V2 += vertexOffset;
				// index.V3 += vertexOffset;

				Triangle triangle;
				triangle.V1 = m_VerticesPositions[index.V1];
				triangle.V2 = m_VerticesPositions[index.V2];
				triangle.V3 = m_VerticesPositions[index.V3];
				m_Triangles.push_back(triangle);

				Triangle triangle2;
				triangle2.V1 = m_VerticesPositions[index.V1 + subMesh.BaseVertex];
				triangle2.V2 = m_VerticesPositions[index.V2 + subMesh.BaseVertex];
				triangle2.V3 = m_VerticesPositions[index.V3 + subMesh.BaseVertex];

				m_TriangleCache[currentMesh].emplace_back(triangle2);

				m_VerticesIndex.push_back(index.V1);
				m_VerticesIndex.push_back(index.V2);
				m_VerticesIndex.push_back(index.V3);

				m_Indices.push_back(index);
			}
			// vertexOffset += mesh->mNumVertices;
		}

		m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_Texcoord" },
			{ ShaderDataType::Int, "a_EntityID" }
		};
		m_VertexBuffer->SetLayout(layout);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		m_IndexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Indices.size() * sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// RNDR_CORE_INFO("Loaded mesh: {0}", filename);


		int vertSize = m_Vertices.size();
		int triSize = m_Triangles.size();
		int indSize = m_Indices.size();
		int posSize = m_VerticesPositions.size();
		int norSize = m_VerticesNormals.size();
		int indPosSize = m_VerticesIndex.size();

		RNDR_CORE_INFO("Vertices: {0}", vertSize);
		RNDR_CORE_INFO("Triangles: {0}", triSize);
		RNDR_CORE_INFO("Indices: {0}", indSize);
		RNDR_CORE_INFO("Positions: {0}", posSize);
		RNDR_CORE_INFO("Normals: {0}", norSize);
		RNDR_CORE_INFO("Index Positions: {0}", indPosSize);

		// exit(0);
	}


	Mesh::~Mesh()
	{
	}

	void Mesh::Render()
	{

	}










	Ref<Mesh> ExtractMesh(aiMesh* aiMesh)
	{
		RNDR_CORE_ASSERT(aiMesh->HasPositions(), "Meshes require positions.");
		RNDR_CORE_ASSERT(aiMesh->HasNormals(), "Meshes require normals.");

		Ref<Mesh> mesh = CreateRef<Mesh>();

		//TODO: posible remove submeshes
		Mesh::SubMesh& subMesh = mesh->m_SubMeshes.emplace_back();
		subMesh.BaseVertex = 0;
		subMesh.BaseIndex = 0;
		subMesh.MaterialIndex = aiMesh->mMaterialIndex;
		subMesh.IndexCount = aiMesh->mNumFaces * 3;

		// Vertices
		mesh->m_Vertices.reserve(aiMesh->mNumVertices);
		mesh->m_VerticesPositions.reserve(aiMesh->mNumVertices);
		mesh->m_VerticesNormals.reserve(aiMesh->mNumVertices);
		for (size_t i = 0; i < aiMesh->mNumVertices; i++)
		{
			Mesh::Vertex vertex;
			vertex.Position = { aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z };
			vertex.Normal = { aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z };

			if (aiMesh->HasTextureCoords(0))
				vertex.Texcoord = { aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y };
			
			mesh->m_Vertices.push_back(vertex);

			mesh->m_VerticesPositions.push_back({ vertex.Position, 1.0f });
			mesh->m_VerticesNormals.push_back({ vertex.Normal, 1.0f });
		}

		// Indices
		mesh->m_Indices.reserve(aiMesh->mNumFaces);
		mesh->m_VerticesIndex.reserve(aiMesh->mNumFaces * 3);
		mesh->m_Triangles.reserve(aiMesh->mNumFaces);
		for (size_t i = 0; i < aiMesh->mNumFaces; i++)
		{
			RNDR_CORE_ASSERT(aiMesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
			Mesh::Index index = { aiMesh->mFaces[i].mIndices[0], aiMesh->mFaces[i].mIndices[1], aiMesh->mFaces[i].mIndices[2] };

			Mesh::Triangle triangle;
			triangle.V1 = mesh->m_VerticesPositions[index.V1];
			triangle.V2 = mesh->m_VerticesPositions[index.V2];
			triangle.V3 = mesh->m_VerticesPositions[index.V3];
			mesh->m_Triangles.push_back(triangle);

			mesh->m_VerticesIndex.push_back(index.V1);
			mesh->m_VerticesIndex.push_back(index.V2);
			mesh->m_VerticesIndex.push_back(index.V3);

			mesh->m_Indices.push_back(index);
		}

		mesh->m_VertexBuffer = VertexBuffer::Create((float*)mesh->m_Vertices.data(), mesh->m_Vertices.size() * sizeof(Mesh::Vertex));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_Texcoord" },
			{ ShaderDataType::Int, "a_EntityID" }
		};
		mesh->m_VertexBuffer->SetLayout(layout);

		mesh->m_VertexArray = VertexArray::Create();
		mesh->m_VertexArray->AddVertexBuffer(mesh->m_VertexBuffer);

		mesh->m_IndexBuffer = IndexBuffer::Create((uint32_t*)mesh->m_Indices.data(), mesh->m_Indices.size() * 3);
		// mesh->m_IndexBuffer = IndexBuffer::Create((uint32_t*)mesh->m_Indices.data(), mesh->m_Indices.size() * sizeof(uint32_t));
		// mesh->m_IndexBuffer = IndexBuffer::Create((uint32_t*)mesh->m_Indices.data(), mesh->m_Indices.size() * sizeof(Mesh::Index));
		mesh->m_VertexArray->SetIndexBuffer(mesh->m_IndexBuffer);

		return mesh;
	}


	void Mesh::ImportMesh(const std::filesystem::path& path, Ref<Scene>& scene)
	{
		LogStream::Initialize();

		RNDR_CORE_INFO("Loading mesh: {0}", path.string().c_str());

		Assimp::Importer importer;

		const aiScene* aiScene = importer.ReadFile(path.string(), ImportFlags);
		if (!scene || !aiScene->HasMeshes())
		{
			RNDR_CORE_ERROR("Failed to load mesh file: {0}", path.string().c_str());
		}

		uint32_t numMeshes = aiScene->mNumMeshes;
		RNDR_CORE_INFO("Number of meshes: {0}", numMeshes);
		UUID parentEntityID = 0;

		// if mesh contains multiple submeshes, we create a parent group entity
		if (numMeshes > 1)
		{
			Entity entity = scene->CreateEntity();
			entity.AddComponent<NullComponent>();
			parentEntityID = entity.GetComponent<IDComponent>().ID;
		}

		for (uint32_t i = 0; i < numMeshes; i++)
		{
			aiMesh* mesh = aiScene->mMeshes[i];
			aiAABB aiAABB = mesh->mAABB;
			const char* meshName = mesh->mName.C_Str();

			Entity entity = scene->CreateEntity(meshName);
			entity.GetComponent<ParentComponent>().Parent = parentEntityID;
			auto& meshComponent = entity.AddComponent<MeshComponent>();
			if (numMeshes > 1)
			{
				Entity parent = scene->GetEntityWithID(parentEntityID);
				parent.GetComponent<ChildComponent>().AddChild(entity.GetComponent<IDComponent>().ID);
			}
			meshComponent.Material->SetShader(Shader::Create("Editor/assets/shaders/MeshShader.glsl"));
			meshComponent.Mesh = ExtractMesh(mesh);
			meshComponent.Mesh->GenerateBVH();
			meshComponent.Min = { aiAABB.mMin.x, aiAABB.mMin.y, aiAABB.mMin.z };
			meshComponent.Max = { aiAABB.mMax.x, aiAABB.mMax.y, aiAABB.mMax.z };
		}

		RNDR_CORE_INFO("Loaded mesh: {0}", path.string().c_str());
	}










	// bvh

	struct BoundingBox
	{
		glm::vec3 Min = glm::vec3(INFINITY);
		glm::vec3 Max = glm::vec3(-INFINITY);

		void Expand(const glm::vec3& point)
		{
			Min = glm::min(Min, point);
			Max = glm::max(Max, point);
		}
	};


	static float NodeCost(glm::vec3 size, int numTriangles)
	{
		// return numTriangles * glm::compAdd(size);
		float halfArea = size.x * size.y + size.x * size.z + size.y * size.z;
		return numTriangles * halfArea;
	}

	float Mesh::EvaluateSplit(int parentIndex, int splitAxis, int splitPos)
	{
		BoundingBox a;
		BoundingBox b;
		int aCount = 0;
		int bCount = 0;

		for (int i = m_BVH[parentIndex].TriangleIndex; i < m_BVH[parentIndex].TriangleIndex + m_BVH[parentIndex].TriangleCount; i++)
		{
			Triangle triangle = m_Triangles[i];
			glm::vec3 centroid = (triangle.V1 + triangle.V2 + triangle.V3) / 3.0f;
			if (centroid[splitAxis] < splitPos)
			{
				a.Expand(triangle.V1);
				a.Expand(triangle.V2);
				a.Expand(triangle.V3);
				aCount++;
			}
			else
			{
				b.Expand(triangle.V1);
				b.Expand(triangle.V2);
				b.Expand(triangle.V3);
				bCount++;
			}
		}
		glm::vec3 sizeA = a.Max - a.Min;
		glm::vec3 sizeB = b.Max - b.Min;
		return NodeCost(sizeA, aCount) + NodeCost(sizeB, bCount);
	}

	static float lerp(float a, float b, float t) {
		return a + t * (b - a);
	}

	std::tuple<int, float, float> Mesh::CalcBestSplit(int parentIndex)
	{
		if (m_BVH[parentIndex].TriangleCount <= 1)
			return std::make_tuple(0, 0.0f, std::numeric_limits<float>::infinity());

		constexpr int numTestsPerAxis = 25;
		float bestCost = std::numeric_limits<float>::infinity();
		float bestPos = 0.0f;
		int bestAxis = 0;

		for (int axis = 0; axis < 3; axis++)
		{
			float boundsStart = m_BVH[parentIndex].Min[axis];
			float boundsEnd = m_BVH[parentIndex].Max[axis];

			for (int i = 0; i < numTestsPerAxis; i++)
			{
				// float splitT = (i + 1) / (float)(numTestsPerAxis + 1);
				float splitT = (i + 1) / (numTestsPerAxis + 1.0f);
				float splitPos = lerp(boundsStart, boundsEnd, splitT);
				float pos = splitPos;
				float cost = EvaluateSplit(parentIndex, axis, pos);



				if (cost < bestCost)
				{
					bestCost = cost;
					bestPos = pos;
					bestAxis = axis;
				}
			}
		}

		return std::make_tuple(bestAxis, bestPos, bestCost);
	}

	static int loadedDepth = 0;

	void Mesh::CalculateAABB(int parentIndex)
	{
		int startIndex = m_BVH[parentIndex].TriangleIndex;
		int TriangleCount = m_BVH[parentIndex].TriangleCount;

		glm::vec3 min = glm::vec3(INFINITY);
		glm::vec3 max = glm::vec3(-INFINITY);
		for (int i = startIndex; i < startIndex + TriangleCount; i++)
		{
			Triangle triangle = m_Triangles[i];
			min = glm::min(min, glm::min(triangle.V1, glm::min(triangle.V2, triangle.V3)));
			max = glm::max(max, glm::max(triangle.V1, glm::max(triangle.V2, triangle.V3)));
		}
		m_BVH[parentIndex].Min = min;
		m_BVH[parentIndex].Max = max;
	}

	void Mesh::SplitBVH(int parentIndex, int depth)
	{
		const int maxDepth = 32;
		if (depth == maxDepth)
			return;

		bvhNode parent = m_BVH[parentIndex];
		if (parent.TriangleCount <= 2)
			return;

		glm::vec3 size = parent.Max - parent.Min;
		int splitAxis = size.x > std::max(size.y, size.z) ? 0 : size.y > size.z ? 1 : 2;
		float splitPos = (parent.Min[splitAxis] + parent.Max[splitAxis]) / 2.0f;

		//TODO: fix SAH, current sersion is slower somehow
		// float parentCost = NodeCost(size, parent.TriangleCount);
		// auto [splitAxis, splitPos, splitCost] = CalcBestSplit(parentIndex);
		// if (splitCost >= parentCost)
		// 	return;

		m_BVH[parentIndex].ChildIndex = m_BVH.size();

		bvhNode childA, childB;
		childA.TriangleIndex = parent.TriangleIndex;
		childA.TriangleCount = 0;
		childB.TriangleIndex = parent.TriangleIndex;
		childB.TriangleCount = 0;

		for (int i = parent.TriangleIndex; i < parent.TriangleIndex + parent.TriangleCount; i++)
		{
			// Triangle triangle = m_Triangles[i];
			// glm::vec3 centroid = (triangle.V1 + triangle.V2 + triangle.V3) / 3.0f;
			// bool isSideA = centroid[splitAxis] < splitPos;

			// bvhNode& child = isSideA ? childA : childB;
			// child.TriangleCount++;

			// if (isSideA)
			// {
			// 	int swap = child.TriangleIndex + child.TriangleCount - 1;
			// 	std::swap(m_Triangles[i], m_Triangles[swap]);
			// 	childB.TriangleIndex++;
			// }
			glm::vec3 V1 = m_Triangles[i].V1;
			glm::vec3 V2 = m_Triangles[i].V2;
			glm::vec3 V3 = m_Triangles[i].V3;

			glm::vec3 center = (V1 + V2 + V3) / 3.0f;

			bool isSideA = center[splitAxis] < splitPos;

			bvhNode& child = isSideA ? childA : childB;
			child.TriangleCount++;

			if (isSideA)
			{
				int swap = child.TriangleIndex + child.TriangleCount - 1;
				std::swap(m_Triangles[i], m_Triangles[swap]);
				childB.TriangleIndex++;
			}
		}
		m_BVH.push_back(childA);
		m_BVH.push_back(childB);

		CalculateAABB(m_BVH[parentIndex].ChildIndex);
		CalculateAABB(m_BVH[parentIndex].ChildIndex + 1);

		if (childA.TriangleCount > 0 && childB.TriangleCount > 0)
		{
			// m_BVH[parentIndex].TriangleCount = 0;

			loadedDepth = depth;
			SplitBVH(m_BVH[parentIndex].ChildIndex, depth + 1);
			SplitBVH(m_BVH[parentIndex].ChildIndex + 1, depth + 1);
		}
	}

	void Mesh::GenerateBVH()
	{
		loadedDepth = 0;
		m_BVH.clear();
		// m_BVH.reserve(m_Triangles.size() / 2);

		bvhNode root;
		root.ChildIndex = 0;
		root.TriangleIndex = 0;
		root.TriangleCount = m_Triangles.size();
		m_BVH.push_back(root);

		CalculateAABB(0);

		SplitBVH(0, 0);

		RNDR_CORE_INFO("BVH depth: {0}", loadedDepth);
	}
}