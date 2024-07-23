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
		RNDR_CORE_INFO("Children: {0}", children);
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

}